#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>

// Função para calcular a média
double calculaMedia(const std::vector<double>& vetor) {
    double soma = 0.0;
    #pragma omp parallel for reduction(+:soma)
    for (size_t i = 0; i < vetor.size(); ++i) {
        soma += vetor[i];
    }
    return soma / vetor.size();
}

// Função para encontrar o maior valor
double maiorValor(const std::vector<double>& vetor) {
    double maiorValor = std::numeric_limits<double>::min();
    #pragma omp parallel for reduction(max:maiorValor)
    for (size_t i = 0; i < vetor.size(); ++i) {
        if (vetor[i] > maiorValor) {
            maiorValor = vetor[i];
        }
    }
    return maiorValor;
}

// Função para calcular o produto
double calculaProduto(const std::vector<double>& vetor) {
    double produto = 1.0;
    #pragma omp parallel for reduction(*:produto)
    for (size_t i = 0; i < vetor.size(); ++i) {
        produto *= vetor[i];
    }
    return produto;
}

int main() {
    // Crie o vetor que preferir para demonstrar a corretude do seu código
    std::vector<double> vetor = {1.0, 2.0, 3.0, 4.0, 5.0};

    double media, maior, produto;

    // execução dessas tarefas simultaneamente e, em cada tarefa, sub-paralelizar para distribuir o processamento dos dados
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            media = calculaMedia(vetor);
        }

        #pragma omp section
        {
            maior = maiorValor(vetor);
        }

        #pragma omp section
        {
            produto = calculaProduto(vetor);
        }
    }
    std::cout << "Média: " << media << std::endl;
    std::cout << "Maior valor: " << maior << std::endl;
    std::cout << "Produto: " << produto << std::endl;
    return 0;
}