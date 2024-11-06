#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include <omp.h>

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

int main() {
    std::string input_dir = "genome_data";
    int num_files_to_process = 2;  // Defina aqui quantos arquivos processar

    // Armazena contagem total de cada base
    std::unordered_map<char, int> base_count = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
    std::vector<std::string> files;

    // Coleta os arquivos até o limite especificado
    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".fa") {
            files.push_back(entry.path().string());
            if (files.size() >= num_files_to_process) break;
        }
    }

    // Processa os arquivos em paralelo
    #pragma omp parallel for shared(base_count)
    for (size_t i = 0; i < files.size(); ++i) {
        std::unordered_map<char, int> local_count = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
        count_bases(files[i], local_count);

        // Mescla a contagem local na contagem total
        #pragma omp critical
        for (const auto& kv : local_count) {
            base_count[kv.first] += kv.second;
        }
    }

    // Exibe o resultado final
    std::cout << "Contagem total de bases:\n";
    for (const auto& kv : base_count) {
        std::cout << kv.first << ": " << kv.second << std::endl;
    }

    return 0;
}