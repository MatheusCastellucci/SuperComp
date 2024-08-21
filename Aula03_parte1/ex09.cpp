#include <iostream>
#include <string>

bool isPalindromo(const std::string& palindromo) {
    int inicio = 0;
    int fim = palindromo.length() - 1;
    
    while (inicio < fim) {
        while (inicio < fim && !std::isalnum(palindromo[inicio])) {
            ++inicio;
        }
        while (inicio < fim && !std::isalnum(palindromo[fim])) {
            --fim;
        }

        // Compara os caracteres em minúsculas
        if (std::tolower(palindromo[inicio]) != std::tolower(palindromo[fim])) {
            return false; // Não é um palíndromo
        }
        ++inicio;
        --fim;
    }
    return true;
}

int main() {
    std::string palavra;

    // Pede ao usuário para inserir uma string
    std::cout << "Digite uma string: ";
    std::getline(std::cin, palavra); // Usa std::getline para ler a linha inteira

    // Verifica se a string é um palíndromo e exibe o resultado
    if (isPalindromo(palavra)) {
        std::cout << "A string eh um palindromo." << std::endl;
    } else {
        std::cout << "A string nao eh um palindromo." << std::endl;
    }

    return 0;
