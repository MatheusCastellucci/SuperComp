#include <iostream>
#include <vector>
#include <chrono>  // Biblioteca para medicao de tempo

// Funcao que aloca dinamicamente uma matriz de inteiros e realiza a soma de seus elementos
void somaMatrizDinamica(int N) {
    // Alocacao dinamica de memoria para uma matriz N x N
    int** matriz = new int*[N];
    for (int i = 0; i < N; ++i) {
        matriz[i] = new int[N];
    }

    // Inicializa a matriz e calcula a soma dos elementos
    int soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;  // Exemplo simples de inicializacao
            soma += matriz[i][j];
        }
    }

    std::cout << "Soma (Alocacao Dinamica): " << soma << std::endl;

    // Liberacao da memoria alocada dinamicamente
    for (int i = 0; i < N; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

// Funcao que utiliza std::vector para criar uma matriz e realizar a soma de seus elementos
void somaMatrizVector(int N) {
    // Uso de std::vector para criar uma matriz N x N
    std::vector<std::vector<int>> matriz(N, std::vector<int>(N));

    // Inicializa a matriz e calcula a soma dos elementos
    int soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;  // Exemplo simples de inicializacao
            soma += matriz[i][j];
        }
    }

    std::cout << "Soma (std::vector): " << soma << std::endl;
}

int main() {
    int N = 1000;  // Define o tamanho da matriz NxN

    // Medindo o tempo de execucao para a alocacao dinamica
    auto inicioDinamica = std::chrono::high_resolution_clock::now();
    somaMatrizDinamica(N);
    auto fimDinamica = std::chrono::high_resolution_clock::now();
    auto duracaoDinamica = std::chrono::duration_cast<std::chrono::milliseconds>(fimDinamica - inicioDinamica).count();
    std::cout << "Tempo de execucao (Alocacao Dinamica): " << duracaoDinamica << " ms" << std::endl;

    // Medindo o tempo de execucao para o uso de std::vector
    auto inicioVector = std::chrono::high_resolution_clock::now();
    somaMatrizVector(N);
    auto fimVector = std::chrono::high_resolution_clock::now();
    auto duracaoVector = std::chrono::duration_cast<std::chrono::milliseconds>(fimVector - inicioVector).count();
    std::cout << "Tempo de execucao (std::vector): " << duracaoVector << " ms" << std::endl;

    return 0;
}
