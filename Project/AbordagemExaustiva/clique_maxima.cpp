#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    // Inicializar matriz de adjacência
    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    // Preencher a matriz de adjacência com as arestas
    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // Grafo não-direcionado
    }

    arquivo.close();
    return grafo;
}

// Função para verificar se um nó pode ser adicionado à clique
bool podeAdicionarClique(const std::vector<int>& clique, int vertice, const std::vector<std::vector<int>>& grafo) {
    for (int v : clique) {
        if (grafo[v][vertice] == 0) {
            return false; // Não há conexão
        }
    }
    return true;
}

// Função recursiva para encontrar a clique máxima
void encontrarCliqueMaxima(int verticeAtual, std::vector<int>& cliqueAtual, 
                           std::vector<int>& melhorClique, const std::vector<std::vector<int>>& grafo) {
    // Atualiza a maior clique encontrada
    if (cliqueAtual.size() > melhorClique.size()) {
        melhorClique = cliqueAtual;
    }

    // Itera sobre os próximos vértices
    for (int v = verticeAtual; v < grafo.size(); ++v) {
        if (podeAdicionarClique(cliqueAtual, v, grafo)) {
            cliqueAtual.push_back(v); // Adiciona o vértice à clique
            encontrarCliqueMaxima(v + 1, cliqueAtual, melhorClique, grafo); // Chamada recursiva
            cliqueAtual.pop_back(); // Remove o vértice ao retornar
        }
    }
}

int main() {
    int numVertices;
    std::vector<std::vector<int>> grafo = LerGrafo("grafo.txt", numVertices);
    
    std::vector<int> melhorClique;
    std::vector<int> cliqueAtual;

    // Executa a busca exaustiva para encontrar a clique máxima
    encontrarCliqueMaxima(0, cliqueAtual, melhorClique, grafo);

    // Imprime a clique máxima encontrada
    std::cout << "Clique máxima encontrada com tamanho " << melhorClique.size() << ": ";
    for (int v : melhorClique) {
        std::cout << v + 1 << " ";
    }
    std::cout << std::endl;

    return 0;
}
