#include <iostream> // Inclui a biblioteca de entrada e saída padrão

int main() {
    double num1, num2; // Declara duas variáveis para armazenar os números
    char operacao; // Declara uma variável para armazenar a operação
    double resultado; // Declara uma variável para armazenar o resultado

    // Pede ao usuário para inserir os números
    std::cout << "Digite o primeiro numero: ";
    std::cin >> num1;

    std::cout << "Digite o segundo numero: ";
    std::cin >> num2;

    // Pede ao usuário para inserir a operação
    std::cout << "Digite a operação (+, -, *, /): ";
    std::cin >> operacao;

    // Verifica qual operação o usuário escolheu e realiza o cálculo
    if (operacao == '+') {
        resultado = num1 + num2;
    } else if (operacao == '-') {
        resultado = num1 - num2;
    } else if (operacao == '*') {
        resultado = num1 * num2;
    } else if (operacao == '/') {
        if (num2 != 0) { // Verifica se o divisor não é zero
            resultado = num1 / num2;
        } else {
            std::cout << "Erro: Divisão por zero não é permitida." << std::endl;
            return 1; // Retorna um código de erro
        }
    } else {
        std::cout << "Operação inválida!" << std::endl;
        return 1; // Retorna um código de erro
    }

    // Exibe o resultado da operação
    std::cout << "Resultado: " << resultado << std::endl;

    return 0; // Indica que o programa terminou com sucesso
}


// outro jeito de fazer
/*
switch (operacao) {
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            if (num2 != 0) { // Verifica se o divisor não é zero
                resultado = num1 / num2;
            } else {
                std::cout << "Erro: Divisão por zero não é permitida." << std::endl;
                return 1; // Retorna um código de erro
            }
            break;
        default:
            std::cout << "Operação inválida!" << std::endl;
            return 1; // Retorna um código de erro
*/