#include <iostream>

int main() {
    int numero;
    std::cout << "Digite um numero: ";
    std::cin >> numero;

    // Verifica se o número é par ou ímpar
    if (numero % 2 == 0) {
        std::cout << "O numero " << numero << " eh par." << std::endl;
    } else {
        std::cout << "O numero " << numero << " eh impar." << std::endl;
    }
    return 0;
}
