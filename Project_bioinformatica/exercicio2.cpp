#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <omp.h>
#include <mpi.h>

namespace fs = std::filesystem;

// Função para transcrever DNA em RNA
std::string transcribe_dna_to_rna(const std::string& dna_sequence) {
    std::string rna_sequence = dna_sequence;
    #pragma omp parallel for
    for (size_t i = 0; i < dna_sequence.size(); ++i) {
        switch (dna_sequence[i]) {
            case 'A': rna_sequence[i] = 'U'; break;
            case 'T': rna_sequence[i] = 'A'; break;
            case 'C': rna_sequence[i] = 'G'; break;
            case 'G': rna_sequence[i] = 'C'; break;
            default: rna_sequence[i] = dna_sequence[i];  // Ignora outros caracteres
        }
    }
    return rna_sequence;
}

// Função para processar um arquivo de DNA e gerar a sequência de RNA correspondente
std::string process_file(const std::string& file) {
    std::ifstream infile(file);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << file << std::endl;
        return "";
    }

    std::string rna_data;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '>') {  // Mantenha linhas de cabeçalho
            rna_data += line + '\n';
        } else {
            rna_data += transcribe_dna_to_rna(line) + '\n';
        }
    }
    infile.close();
    return rna_data;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::string input_dir = "processed_data";
    std::string output_dir = "rna_data";
    int num_files_to_process = 22;  // Defina quantos arquivos processar
    std::vector<std::string> all_files;
    
    if (world_rank == 0) {
        fs::create_directory(output_dir);  // Cria diretório de saída se não existir
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

    // Processamento de transcrição de DNA para RNA em cada arquivo
    std::vector<std::string> rna_results;
    for (const auto& file : local_files) {
        rna_results.push_back(process_file(file));
    }

    // Envia os resultados de RNA para o processo mestre
    if (world_rank == 0) {
        for (size_t i = 0; i < local_files.size(); ++i) {
            std::string output_file = output_dir + "/" + fs::path(local_files[i]).stem().string() + "_rna.fa";
            std::ofstream outfile(output_file);
            outfile << rna_results[i];
            outfile.close();
            std::cout << "Arquivo RNA gerado: " << output_file << std::endl;
        }

        for (int i = 1; i < world_size; i++) {
            int files_to_receive;
            MPI_Recv(&files_to_receive, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < files_to_receive; j++) {
                char file_name[256];
                MPI_Recv(file_name, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int rna_size;
                MPI_Recv(&rna_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::string rna_data(rna_size, ' ');
                MPI_Recv(&rna_data[0], rna_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                std::string output_file = output_dir + "/" + fs::path(file_name).stem().string() + "_rna.fa";
                std::ofstream outfile(output_file);
                outfile << rna_data;
                outfile.close();
                std::cout << "Arquivo RNA gerado: " << output_file << std::endl;
            }
        }
    } else {
        int files_to_send = local_files.size();
        MPI_Send(&files_to_send, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for (size_t i = 0; i < local_files.size(); ++i) {
            MPI_Send(local_files[i].c_str(), local_files[i].size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            int rna_size = rna_results[i].size();
            MPI_Send(&rna_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(rna_results[i].c_str(), rna_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
