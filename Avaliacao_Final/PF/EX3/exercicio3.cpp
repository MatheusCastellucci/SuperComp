#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>

// Função para calcular a média
double calculaMedia(const std::vector<double>& numbers) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < numbers.size(); ++i) {
        sum += numbers[i];
    }
    sum /= numbers.size();
    return sum;
}

// Função para encontrar o maior valor
double maiorValor(const std::vector<double>& numbers) {
    double maxVal = std::numeric_limits<double>::min();
    #pragma omp parallel for reduction(max:maxVal)
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] > maxVal) {
            maxVal = numbers[i];
        }
    }
    return maxVal;
}

// Função para calcular o produto
double calculaProduto(const std::vector<double>& numbers) {
    double product = 1.0;
        
    for (size_t i = 0; i < numbers.size(); ++i) {
        product *= numbers[i];
    }
    return product;
}

int main() {
    // Crie o vetor que preferir para demonstrar a corretude do seu código
    std::vector<double> numbers = {1.0, 2.0, 3.0, 4.0, 5.0};

    double average, maxVal, product;

    // Paralelizando a execução das três tarefas
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            average = calculaMedia(numbers);
        }

        #pragma omp section
        {
            maxVal = maiorValor(numbers);
        }

        #pragma omp section
        {
            product = calculaProduto(numbers);
        }
    }

    // Exibindo os resultados
    std::cout << "Média: " << average << std::endl;
    std::cout << "Maior valor: " << maxVal << std::endl;
    std::cout << "Produto: " << product << std::endl;

    return 0;
}