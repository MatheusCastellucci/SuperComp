#include <iostream>
#include <string>

bool isPalindromo(const std::string& str) {
    int inicio = 0;
    int fim = str.length() - 1;
    
    while (inicio < fim) {
        while (inicio < fim && !std::isalnum(str[inicio])) {
            ++inicio;
        }
        while (inicio < fim && !std::isalnum(str[fim])) {
            --fim;
        }

        // Compara os caracteres em minúsculas
        if (std::tolower(str[inicio]) != std::tolower(str[fim])) {
            return false; // Não é um palíndromo
        }
        ++inicio;
        --fim;
    }
    
    return true; // É um palíndromo
}

int main() {
    std::string palavra; // Declara uma variável para armazenar a string

    // Pede ao usuário para inserir uma string
    std::cout << "Digite uma string: ";
    std::getline(std::cin, palavra); // Usa std::getline para ler a linha inteira

    // Verifica se a string é um palíndromo e exibe o resultado
    if (isPalindromo(palavra)) {
        std::cout << "A string eh um palindromo." << std::endl;
    } else {
        std::cout << "A string nao eh um palindromo." << std::endl;
    }

    return 0; // Indica que o programa terminou com sucesso
}
