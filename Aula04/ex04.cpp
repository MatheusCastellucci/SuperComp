#include <iostream>
#include <string>   // Inclui a biblioteca para manipulação de strings

int main() {
    std::string entrada; // Declara uma variável para armazenar a string

    // Pede ao usuário para inserir uma string
    std::cout << "Digite uma string: ";
    std::getline(std::cin, entrada); // Usa std::getline para ler uma linha inteira, incluindo espaços

    // Exibe o número de caracteres na string
    std::cout << "O numero de caracteres na string eh: " << entrada.length() << std::endl;

    return 0; // Indica que o programa terminou com sucesso
}
