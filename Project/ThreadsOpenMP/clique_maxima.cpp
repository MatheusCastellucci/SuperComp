#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <omp.h>  // Inclui a biblioteca OpenMP

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;
    }

    arquivo.close();
    return grafo;
}

// Função para verificar se um nó pode ser adicionado à clique
bool podeAdicionarClique(const std::vector<int>& clique, int vertice, const std::vector<std::vector<int>>& grafo) {
    for (int v : clique) {
        if (grafo[v][vertice] == 0) {
            return false;
        }
    }
    return true;
}

// Função recursiva para encontrar a clique máxima
void encontrarCliqueMaxima(int verticeAtual, std::vector<int>& cliqueAtual, 
                           std::vector<int>& melhorClique, const std::vector<std::vector<int>>& grafo) {
    if (cliqueAtual.size() > melhorClique.size()) {
        melhorClique = cliqueAtual;
    }

    for (int v = verticeAtual; v < grafo.size(); ++v) {
        if (podeAdicionarClique(cliqueAtual, v, grafo)) {
            cliqueAtual.push_back(v);
            encontrarCliqueMaxima(v + 1, cliqueAtual, melhorClique, grafo);
            cliqueAtual.pop_back();
        }
    }
}

int main() {
    int numVertices;
    std::vector<std::vector<int>> grafo = LerGrafo("grafo.txt", numVertices);

    std::vector<int> melhorCliqueGlobal;
    
    // Define o paralelismo para cada vértice inicial
    #pragma omp parallel
    {
        std::vector<int> melhorCliqueLocal;
        std::vector<int> cliqueAtual;

        #pragma omp for
        for (int i = 0; i < numVertices; ++i) {
            cliqueAtual.push_back(i);
            encontrarCliqueMaxima(i + 1, cliqueAtual, melhorCliqueLocal, grafo);
            cliqueAtual.pop_back();

            // Atualiza a melhor clique global se necessário
            #pragma omp critical
            {
                if (melhorCliqueLocal.size() > melhorCliqueGlobal.size()) {
                    melhorCliqueGlobal = melhorCliqueLocal;
                }
            }
        }
    }

    // Imprime a clique máxima encontrada
    std::cout << "Clique máxima encontrada com tamanho " << melhorCliqueGlobal.size() << ": ";
    for (int v : melhorCliqueGlobal) {
        std::cout << v + 1 << " ";
    }
    std::cout << std::endl;

    return 0;
}
