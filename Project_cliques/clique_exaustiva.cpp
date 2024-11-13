#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    
    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo " << nomeArquivo << std::endl;
        return {};
    } else {
        std::cout << "Arquivo " << nomeArquivo << " encontrado e aberto com sucesso." << std::endl;
    }
    
    int numArestas;
    if (!(arquivo >> numVertices >> numArestas)) {
        std::cerr << "Erro ao ler o número de vértices e arestas." << std::endl;
        return {};
    }
    
    std::cout << "Número de vértices: " << numVertices << ", Número de arestas: " << numArestas << std::endl;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    // Lê as arestas e exibe cada linha lida
    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        if (!(arquivo >> u >> v)) {
            std::cerr << "Erro ao ler uma aresta." << std::endl;
            return {};
        }
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // Grafo não direcionado
        
        // Exibe a aresta lida
        std::cout << "Aresta adicionada: " << u << " <-> " << v << std::endl;
    }

    arquivo.close();
    return grafo;
}

bool FormaClique(const std::vector<std::vector<int>>& grafo, const std::vector<int>& conjunto) {
    for (size_t i = 0; i < conjunto.size(); ++i) {
        for (size_t j = i + 1; j < conjunto.size(); ++j) {
            if (grafo[conjunto[i]][conjunto[j]] == 0) {
                return false;
            }
        }
    }
    return true;
}

std::vector<int> EncontrarCliqueMaxima(const std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> cliqueMaxima;

    for (int mask = 0; mask < (1 << numVertices); ++mask) {
        std::vector<int> conjunto;
        for (int i = 0; i < numVertices; ++i) {
            if (mask & (1 << i)) {
                conjunto.push_back(i);
            }
        }

        if (FormaClique(grafo, conjunto) && conjunto.size() > cliqueMaxima.size()) {
            cliqueMaxima = conjunto;
        }
    }

    return cliqueMaxima;
}

int main() {
    std::string nomeArquivo = "grafo.txt";
    int numVertices;
    auto grafo = LerGrafo(nomeArquivo, numVertices);

    if (grafo.empty()) {
        std::cerr << "Erro ao carregar o grafo." << std::endl;
        return 1;
    }

    auto inicio = std::chrono::high_resolution_clock::now();

    auto cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Clique máxima encontrada: ";
    for (int v : cliqueMaxima) {
        std::cout << v + 1 << " ";
    }
    std::cout << "\nTamanho da clique máxima: " << cliqueMaxima.size() << std::endl;
    std::cout << "Tempo de execução: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
