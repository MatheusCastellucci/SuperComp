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

// Função recursiva para encontrar a clique máxima com poda
void encontrarCliqueMaxima(int verticeAtual, std::vector<int>& cliqueAtual, 
                           std::vector<int>& melhorClique, const std::vector<std::vector<int>>& grafo) {
    // Atualiza a maior clique encontrada até o momento
    if (cliqueAtual.size() > melhorClique.size()) {
        melhorClique = cliqueAtual;
    }

    // Verifica se continuar a busca é viável
    if (cliqueAtual.size() + (grafo.size() - verticeAtual) <= melhorClique.size()) {
        return; // Poda por tamanho: Não há como formar uma clique maior a partir daqui
    }

    // Itera sobre os próximos vértices
    for (int v = verticeAtual; v < grafo.size(); ++v) {
        if (podeAdicionarClique(cliqueAtual, v, grafo)) {
            cliqueAtual.push_back(v);
            encontrarCliqueMaxima(v + 1, cliqueAtual, melhorClique, grafo); // Chamada recursiva
            cliqueAtual.pop_back();
        }
    }
}

int main() {
    int numVertices;
    std::vector<std::vector<int>> grafo = LerGrafo("grafo.txt", numVertices);
    
    std::vector<int> melhorClique;
    std::vector<int> cliqueAtual;

    // Ordena os vértices por grau de adjacência
    std::vector<int> vertices(numVertices);
    for (int i = 0; i < numVertices; ++i) vertices[i] = i;
    std::sort(vertices.begin(), vertices.end(), [&](int a, int b) {
        return std::count(grafo[a].begin(), grafo[a].end(), 1) > std::count(grafo[b].begin(), grafo[b].end(), 1);
    });

    // Executa a busca exaustiva aprimorada para encontrar a clique máxima
    encontrarCliqueMaxima(0, cliqueAtual, melhorClique, grafo);

    // Imprime a clique máxima encontrada
    std::cout << "Clique máxima encontrada com tamanho " << melhorClique.size() << ": ";
    for (int v : melhorClique) {
        std::cout << v + 1 << " ";
    }
    std::cout << std::endl;

    return 0;
}
