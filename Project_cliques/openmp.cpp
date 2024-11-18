#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <omp.h>

using namespace std;

// Função para ler o grafo a partir do arquivo de entrada
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();
    return grafo;
}

// Função para verificar se um vértice pode ser adicionado ao clique atual
bool PodeAdicionar(const vector<int>& cliqueAtual, int v, const vector<vector<int>>& grafo) {
    for (int u : cliqueAtual) {
        if (grafo[u][v] == 0) return false;
    }
    return true;
}

// Função para encontrar cliques maximais em paralelo
vector<vector<int>> EncontrarCliquesMaximais(const vector<vector<int>>& grafo, int numVertices) {
    vector<vector<int>> cliquesMaximais;

    #pragma omp parallel
    {
        vector<vector<int>> cliquesLocais;

        #pragma omp for schedule(dynamic)
        for (int v = 0; v < numVertices; ++v) {
            vector<int> cliqueAtual = {v};

            for (int u = v + 1; u < numVertices; ++u) {
                if (PodeAdicionar(cliqueAtual, u, grafo)) {
                    cliqueAtual.push_back(u);
                }
            }

            #pragma omp critical
            cliquesLocais.push_back(cliqueAtual);
        }

        #pragma omp critical
        cliquesMaximais.insert(cliquesMaximais.end(), cliquesLocais.begin(), cliquesLocais.end());
    }

    return cliquesMaximais;
}

// Função para encontrar a maior clique (clique máxima)
vector<int> EncontrarCliqueMaxima(const vector<vector<int>>& cliquesMaximais) {
    vector<int> cliqueMaxima;

    #pragma omp parallel for
    for (size_t i = 0; i < cliquesMaximais.size(); ++i) {
        #pragma omp critical
        if (cliquesMaximais[i].size() > cliqueMaxima.size()) {
            cliqueMaxima = cliquesMaximais[i];
        }
    }

    return cliqueMaxima;
}

int main() {
    string nomeArquivo = "grafo.txt"; // Nome do arquivo de entrada
    int numVertices;

    // Lê o grafo do arquivo
    vector<vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    // Medindo o tempo de execução
    double inicio = omp_get_wtime();

    // Encontra cliques maximais e a clique máxima
    vector<vector<int>> cliquesMaximais = EncontrarCliquesMaximais(grafo, numVertices);
    vector<int> cliqueMaxima = EncontrarCliqueMaxima(cliquesMaximais);

    double fim = omp_get_wtime();

    // Exibe o resultado
    cout << "Clique Máxima Encontrada: ";
    for (int v : cliqueMaxima) {
        cout << (v + 1) << " "; // Ajuste para 1-based index
    }
    cout << endl;

    cout << "Tamanho da Clique Máxima: " << cliqueMaxima.size() << endl;
    cout << "Tempo de Execução: " << (fim - inicio) << " segundos" << endl;

    return 0;
}
