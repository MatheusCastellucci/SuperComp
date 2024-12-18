#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include <omp.h>
#include <mpi.h>

namespace fs = std::filesystem;

// Função para contar as bases em um arquivo
void count_bases(const std::string& file, std::unordered_map<char, int>& base_count) {
    std::ifstream infile(file);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '>') continue;  // Ignorar linhas vazias e cabeçalhos

        for (char ch : line) {
            ch = std::toupper(ch);
            if (ch == 'A' || ch == 'T' || ch == 'C' || ch == 'G') {
                #pragma omp atomic
                base_count[ch]++;
            }
        }
    }
    infile.close();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::string input_dir = "processed_data";
    int num_files_to_process = 22;  // Defina quantos arquivos processar
    std::vector<std::string> all_files;
    
    // O processo mestre coleta os arquivos e distribui para os processos
    if (world_rank == 0) {
        for (const auto& entry : fs::directory_iterator(input_dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".fa") {
                all_files.push_back(entry.path().string());
                if (all_files.size() >= num_files_to_process) break;
            }
        }
    }

    // Transmite o número total de arquivos para todos os processos
    int total_files = all_files.size();
    MPI_Bcast(&total_files, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Divide os arquivos entre processos
    int files_per_process = (total_files + world_size - 1) / world_size;
    std::vector<std::string> local_files;

    if (world_rank == 0) {
        // Envia as partes dos arquivos para cada processo
        for (int i = 1; i < world_size; i++) {
            int start = i * files_per_process;
            int end = std::min(start + files_per_process, total_files);
            for (int j = start; j < end; j++) {
                MPI_Send(all_files[j].c_str(), all_files[j].size() + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
        local_files.assign(all_files.begin(), all_files.begin() + files_per_process);
    } else {
        for (int i = 0; i < files_per_process && world_rank * files_per_process + i < total_files; i++) {
            char file_name[256];
            MPI_Recv(file_name, 256, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_files.emplace_back(file_name);
        }
    }

    // Contagem de bases para cada arquivo
    std::vector<std::unordered_map<char, int>> file_counts(local_files.size());

    #pragma omp parallel for
    for (size_t i = 0; i < local_files.size(); ++i) {
        file_counts[i] = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
        count_bases(local_files[i], file_counts[i]);
    }

    // Processo mestre coleta e exibe os resultados de cada processo
    if (world_rank == 0) {
        // Exibir contagens locais do processo mestre
        for (size_t i = 0; i < local_files.size(); ++i) {
            std::cout << "Arquivo: " << local_files[i] << "\n";
            std::cout << "A: " << file_counts[i]['A'] << ", T: " << file_counts[i]['T']
                      << ", C: " << file_counts[i]['C'] << ", G: " << file_counts[i]['G'] << "\n";
        }

        // Receber e exibir contagens de outros processos
        for (int i = 1; i < world_size; i++) {
            int files_to_receive;
            MPI_Recv(&files_to_receive, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < files_to_receive; j++) {
                char file_name[256];
                int counts[4];
                MPI_Recv(file_name, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(counts, 4, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                std::cout << "Arquivo: " << file_name << "\n";
                std::cout << "A: " << counts[0] << ", T: " << counts[1]
                          << ", C: " << counts[2] << ", G: " << counts[3] << "\n";
            }
        }
    } else {
        // Enviar o número de arquivos e suas contagens para o processo mestre
        int files_to_send = local_files.size();
        MPI_Send(&files_to_send, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        for (size_t i = 0; i < local_files.size(); ++i) {
            int counts[4] = {file_counts[i]['A'], file_counts[i]['T'], file_counts[i]['C'], file_counts[i]['G']};
            MPI_Send(local_files[i].c_str(), local_files[i].size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            MPI_Send(counts, 4, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
