#include <iostream>
#include <fstream>
#include <filesystem>
#include <cctype>

namespace fs = std::filesystem;

void preprocess_fasta(const std::string& input_file, const std::string& output_file) {
    std::ifstream infile(input_file);
    std::ofstream outfile(output_file);

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << input_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;

        // Mantenha as linhas de cabeçalho
        if (line[0] == '>') {
            outfile << line << '\n';
        } else {
            // Processa as linhas de sequência
            for (char& ch : line) {
                ch = std::toupper(ch);  // Converte para maiúscula
                if (ch == 'A' || ch == 'T' || ch == 'C' || ch == 'G') {
                    outfile << ch;  // Apenas escreve as bases válidas
                }
            }
            outfile << '\n';  // Nova linha após cada sequência processada
        }
    }

    infile.close();
    outfile.close();
    std::cout << "Pré-processamento concluído para " << input_file << std::endl;
}

int main() {
    std::string input_dir = "genome_data";
    std::string output_dir = "processed_data";
    fs::create_directory(output_dir);

    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".fa") {
            std::string input_file = entry.path().string();
            std::string output_file = output_dir + "/" + entry.path().stem().string() + "_processed.fa";
            preprocess_fasta(input_file, output_file);
        }
    }

    std::cout << "Todos os arquivos foram processados e salvos em " << output_dir << std::endl;
    return 0;
}
