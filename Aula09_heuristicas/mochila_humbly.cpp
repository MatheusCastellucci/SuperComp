#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>  // Para medir o tempo de execução

// Definição de uma struct para armazenar os dados dos objetos
struct Objeto {
    int peso;
    int valor;
};

// Função para comparar objetos pelo peso (ordem crescente)
bool compararPorPeso(const Objeto &a, const Objeto &b) {
    return a.peso < b.peso;
}

// Função para selecionar os objetos de acordo com a heurística "mais leve primeiro"
int selecionarObjetosMaisLeves(std::vector<Objeto> &objetos, int capacidadeMaxima) {
    // Ordenar os objetos pelo peso em ordem crescente
    std::sort(objetos.begin(), objetos.end(), compararPorPeso);
    
    int valorTotal = 0;
    int pesoAtual = 0;

    // Selecionar os objetos dentro da capacidade máxima
    for (const auto &objeto : objetos) {
        if (pesoAtual + objeto.peso <= capacidadeMaxima) {
            pesoAtual += objeto.peso;
            valorTotal += objeto.valor;
        }
    }

    return valorTotal;
}

int main() {
    std::vector<Objeto> objetos;
    int capacidadeMaxima;

    // Abrir o arquivo de entrada
    std::ifstream entrada("Entrada_3.txt");

    if (!entrada) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }

    // Ler a capacidade máxima da primeira linha
    entrada >> capacidadeMaxima;

    // Ler os pesos e valores do arquivo e armazenar no vetor de objetos
    int peso, valor;
    while (entrada >> peso >> valor) {
        objetos.push_back({peso, valor});
    }

    entrada.close();  // Fechar o arquivo após a leitura

    // Medir o tempo de execução
    auto inicio = std::chrono::high_resolution_clock::now();

    // Selecionar os objetos de acordo com a heurística "mais leve primeiro"
    int valorMaximo = selecionarObjetosMaisLeves(objetos, capacidadeMaxima);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    // Exibir os resultados
    std::cout << "Valor total dos objetos selecionados: " << valorMaximo << std::endl;
    std::cout << "Tempo de execução: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
