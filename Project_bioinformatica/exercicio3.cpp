#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <omp.h>
#include <mpi.h>

namespace fs = std::filesystem;

// Função para contar o número de códons "AUG" em uma sequência de RNA
int count_protein_initializations(const std::string& rna_sequence) {
    int count = 0;
    #pragma omp parallel for reduction(+:count)
    for (size_t i = 0; i < rna_sequence.size() - 2; i += 3) {
        if (rna_sequence.substr(i, 3) == "AUG") {
            count++;
        }
    }
    return count;
}

// Função para processar um arquivo de RNA e contar as proteínas inicializadas
int process_file(const std::string& file) {
    std::ifstream infile(file);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << file << std::endl;
        return 0;
    }

    int local_count = 0;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '>') continue;  // Ignora linhas de cabeçalho
        local_count += count_protein_initializations(line);
    }
    infile.close();
    return local_count;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::string input_dir = "rna_data";
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

    // Broadcast do número total de arquivos para todos os processos
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

    // Contagem de proteínas inicializadas para cada arquivo individualmente
    std::vector<int> local_protein_counts;
    for (const auto& file : local_files) {
        int count = process_file(file);
        local_protein_counts.push_back(count);
    }

    // Processo mestre coleta e exibe os resultados de cada processo
    if (world_rank == 0) {
        // Exibe as contagens locais do processo mestre
        for (size_t i = 0; i < local_files.size(); ++i) {
            std::cout << "Arquivo: " << local_files[i] << " - Proteínas inicializadas (AUG): " << local_protein_counts[i] << "\n";
        }

        // Recebe e exibe as contagens de proteínas inicializadas de outros processos
        for (int i = 1; i < world_size; i++) {
            int files_to_receive;
            MPI_Recv(&files_to_receive, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < files_to_receive; j++) {
                char file_name[256];
                int count;
                MPI_Recv(file_name, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                std::cout << "Arquivo: " << file_name << " - Proteínas inicializadas (AUG): " << count << "\n";
            }
        }
    } else {
        // Envia o número de arquivos e as contagens para o processo mestre
        int files_to_send = local_files.size();
        MPI_Send(&files_to_send, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        for (size_t i = 0; i < local_files.size(); ++i) {
            MPI_Send(local_files[i].c_str(), local_files[i].size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&local_protein_counts[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
