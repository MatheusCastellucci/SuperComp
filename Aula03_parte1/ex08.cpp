#include <iostream>

int main() {
    const int tamanho = 3; // Define o tamanho da matriz 3x3
    int matriz[tamanho][tamanho]; // Declara a matriz 3x3
    int somaDiagonal = 0;

    // Pede ao usuário para inserir os elementos da matriz
    std::cout << "Digite os elementos da matriz 3x3:" << std::endl;
    for (int i = 0; i < tamanho; ++i) {
        for (int j = 0; j < tamanho; ++j) {
            std::cout << "Elemento [" << i << "][" << j << "]: ";
            std::cin >> matriz[i][j]; // Lê o elemento da matriz
        }
    }

    // Calcula a soma dos elementos na diagonal principal
    for (int i = 0; i < tamanho; ++i) {
        somaDiagonal += matriz[i][i]; // Adiciona o elemento da diagonal principal
    }
    std::cout << "A soma dos elementos na diagonal principal eh: " << somaDiagonal << std::endl;
    return 0;
}
