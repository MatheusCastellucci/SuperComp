#include <iostream>
#include <fstream>  // Inclui a biblioteca para operações de arquivo
#include <sstream>  // Inclui a biblioteca para manipulação de strings
#include <string>
#include <unordered_map> // Inclui a biblioteca para mapear palavras e contar frequências
#include <vector>   // Inclui a biblioteca para manipulação de vetores
#include <algorithm> // Inclui a biblioteca para operações algorítmicas como max_element

// Função para calcular estatísticas de texto
void calcularEstatisticas(const std::string& arquivoEntrada, const std::string& arquivoSaida) {
    std::ifstream inputFile(arquivoEntrada);
    if (!inputFile.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo de entrada." << std::endl;
        return;
    }

    std::unordered_map<std::string, int> palavraContagem; // Para contar a frequência das palavras
    std::string linha;
    int totalLinhas = 0;
    int totalPalavras = 0;
    int totalPalavrasPorLinha = 0;

    while (std::getline(inputFile, linha)) {
        totalLinhas++;
        std::istringstream iss(linha);
        std::string palavra;
        int palavrasNaLinha = 0;

        while (iss >> palavra) {
            palavraContagem[palavra]++;
            palavrasNaLinha++;
            totalPalavras++;
        }

        totalPalavrasPorLinha += palavrasNaLinha;
    }

    inputFile.close();

    // Calcula o número médio de palavras por linha
    double mediaPalavrasPorLinha = (totalLinhas > 0) ? static_cast<double>(totalPalavrasPorLinha) / totalLinhas : 0.0;

    // Encontra a palavra mais frequente
    std::string palavraMaisFrequente;
    int maxFrequencia = 0;
    for (const auto& par : palavraContagem) {
        if (par.second > maxFrequencia) {
            maxFrequencia = par.second;
            palavraMaisFrequente = par.first;
        }
    }

    // Grava as estatísticas em um novo arquivo
    std::ofstream outputFile(arquivoSaida);
    if (!outputFile.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo de saída." << std::endl;
        return;
    }

    outputFile << "Número total de palavras: " << totalPalavras << std::endl;
    outputFile << "Número total de linhas: " << totalLinhas << std::endl;
    outputFile << "Número médio de palavras por linha: " << mediaPalavrasPorLinha << std::endl;
    outputFile << "Palavra mais frequente: " << palavraMaisFrequente << std::endl;

    outputFile.close();
}

int main() {
    std::string arquivoEntrada;
    std::string arquivoSaida;

    // Solicita ao usuário os nomes dos arquivos
    std::cout << "Digite o nome do arquivo de entrada: ";
    std::cin >> arquivoEntrada;
    std::cout << "Digite o nome do arquivo de saída: ";
    std::cin >> arquivoSaida;

    // Calcula as estatísticas e grava no arquivo de saída
    calcularEstatisticas(arquivoEntrada, arquivoSaida);

    std::cout << "Estatísticas calculadas e gravadas com sucesso." << std::endl;

    return 0;
}
