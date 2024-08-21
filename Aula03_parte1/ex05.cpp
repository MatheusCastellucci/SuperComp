#include <iostream>

int main() {
    const int tamanho = 5; // Define o tamanho do vetor   -   Infelizmente precisa fazer separado
    int numeros[tamanho];  // Declara um vetor para armazenar os números
    int soma = 0;

    // Pede ao usuário para inserir 5 números
    std::cout << "Digite 5 numeros:" << std::endl;
    for (int i = 0; i < tamanho; ++i) {
        std::cout << "Numero " << (i + 1) << ": ";
        std::cin >> numeros[i]; // Lê o número e armazena no vetor
    }

    // Calcula a soma dos números
    for (int i = 0; i < tamanho; ++i) {
        soma += numeros[i]; // Adiciona o número ao total da soma
    }

    // Exibe a soma dos números
    std::cout << "A soma dos numeros eh: " << soma << std::endl;

    return 0;
}
