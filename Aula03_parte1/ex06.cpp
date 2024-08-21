#include <iostream> 

int main() {
    const int tamanho = 10;
    int numeros[tamanho];

    // Pede ao usuário para inserir 10 números
    std::cout << "Digite 10 numeros:" << std::endl;
    for (int i = 0; i < tamanho; ++i) {
        std::cout << "Numero " << (i + 1) << ": ";
        std::cin >> numeros[i]; // Lê o número e armazena no vetor
    }

    // Determina o maior número no vetor
    int maior = numeros[0]; // Assume que o primeiro número é o maior inicialmente
    for (int i = 1; i < tamanho; ++i) {
        if (numeros[i] > maior) {
            maior = numeros[i]; // Atualiza o maior número se o número atual for maior
        }
    }

    // Exibe o maior número
    std::cout << "O maior numero eh: " << maior << std::endl;

    return 0;
}
