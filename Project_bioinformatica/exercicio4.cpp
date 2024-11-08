#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <omp.h>
#include <mpi.h>

namespace fs = std::filesystem;

// Mapeamento de códons para aminoácidos (representados por nomes)
const std::unordered_map<std::string, std::string> codon_to_aminoacid = {
    {"CCA", "Prolina"}, {"CCG", "Prolina"}, {"CCU", "Prolina"}, {"CCC", "Prolina"},
    {"UCU", "Serina"}, {"UCA", "Serina"}, {"UCG", "Serina"}, {"UCC", "Serina"},
    {"CAG", "Glutamina"}, {"CAA", "Glutamina"},
    {"ACA", "Treonina"}, {"ACC", "Treonina"}, {"ACU", "Treonina"}, {"ACG", "Treonina"},
    {"AUG", "Metionina"},   // Códon de início
    {"UGA", "STOP"}, {"UAA", "STOP"}, {"UAG", "STOP"},
    {"UGC", "Cisteína"}, {"UGU", "Cisteína"},
    {"GUG", "Valina"}, {"GUA", "Valina"}, {"GUC", "Valina"}, {"GUU", "Valina"}
};

// Função para contar aminoácidos em uma sequência de RNA até encontrar um códon de parada
std::unordered_map<std::string, int> count_aminoacids(const std::string& rna_sequence) {
    std::unordered_map<std::string, int> aminoacid_counts;
    bool stop_found = false;

    #pragma omp parallel for shared(aminoacid_counts, stop_found)
    for (size_t i = 0; i < rna_sequence.size() - 2; i += 3) {
        if (stop_found) continue;

        std::string codon = rna_sequence.substr(i, 3);
        if (codon_to_aminoacid.count(codon)) {
            std::string aminoacid = codon_to_aminoacid.at(codon);

            if (aminoacid == "STOP") { // Códon de parada
                stop_found = true;
            } else {
                #pragma omp critical
                aminoacid_counts[aminoacid]++;
            }
        }
    }

    return aminoacid_counts;
}

// Função para processar um arquivo de RNA e contar os aminoácidos
std::unordered_map<std::string, int> process_file(const std::string& file) {
    std::ifstream infile(file);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << file << std::endl;
        return {};
    }

    std::unordered_map<std::string, int> aminoacid_counts;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '>') continue; // Ignora linhas de cabeçalho

        auto line_counts = count_aminoacids(line);
        for (const auto& [name, count] : line_counts) {
            aminoacid_counts[name] += count;
        }
    }
    infile.close();
    return aminoacid_counts;
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

    // Contagem de aminoácidos para cada arquivo individualmente
    std::vector<std::pair<std::string, std::unordered_map<std::string, int>>> local_file_aminoacid_counts;
    for (const auto& file : local_files) {
        auto file_counts = process_file(file);
        local_file_aminoacid_counts.emplace_back(file, file_counts);
    }

    // Processo mestre coleta e exibe os resultados de cada processo
    if (world_rank == 0) {
        // Exibe as contagens de aminoácidos para cada arquivo processado pelo mestre
        for (const auto& [file, counts] : local_file_aminoacid_counts) {
            std::cout << "Arquivo: " << file << "\n";
            for (const auto& [aminoacid, count] : counts) {
                std::cout << "  " << aminoacid << ": " << count << "\n";
            }
        }

        // Recebe e exibe as contagens de outros processos
        for (int i = 1; i < world_size; i++) {
            int files_to_receive;
            MPI_Recv(&files_to_receive, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < files_to_receive; j++) {
                char file_name[256];
                MPI_Recv(file_name, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                int num_aminoacids;
                MPI_Recv(&num_aminoacids, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                std::cout << "Arquivo: " << file_name << "\n";
                for (int k = 0; k < num_aminoacids; k++) {
                    char aminoacid_name[256];
                    int count;
                    MPI_Recv(aminoacid_name, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    std::cout << "  " << aminoacid_name << ": " << count << "\n";
                }
            }
        }
    } else {
        // Envia a contagem de aminoácidos para cada arquivo para o processo mestre
        int files_to_send = local_file_aminoacid_counts.size();
        MPI_Send(&files_to_send, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        for (const auto& [file, counts] : local_file_aminoacid_counts) {
            MPI_Send(file.c_str(), file.size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

            int num_aminoacids = counts.size();
            MPI_Send(&num_aminoacids, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            for (const auto& [aminoacid, count] : counts) {
                MPI_Send(aminoacid.c_str(), aminoacid.size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
