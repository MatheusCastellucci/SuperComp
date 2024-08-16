#include <iostream> // Inclui a biblioteca de entrada e saída padrão

int main() {
    int numero; // Declara uma variável para armazenar o número

    // Pede ao usuário para inserir um número
    std::cout << "Digite um numero: ";
    std::cin >> numero;

    // Verifica se o número é par ou ímpar
    if (numero % 2 == 0) {
        std::cout << "O numero " << numero << " eh par." << std::endl;
    } else {
        std::cout << "O numero " << numero << " eh impar." << std::endl;
    }

    return 0; // Indica que o programa terminou com sucesso
}
