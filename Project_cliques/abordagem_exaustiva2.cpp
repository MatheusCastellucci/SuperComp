#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Variáveis globais
vector<vector<int>> grafo;
vector<int> melhorClique;
int numVertices;

// Função para ler o grafo a partir do arquivo de entrada
void LerGrafo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    grafo.assign(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();
}

// Função para calcular o grau de cada vértice
vector<int> CalcularGrau() {
    vector<int> grau(numVertices, 0);
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            grau[i] += grafo[i][j];
        }
    }
    return grau;
}

// Função para verificar se um vértice pode ser adicionado ao clique atual
bool PodeAdicionar(const vector<int>& cliqueAtual, int v) {
    for (int u : cliqueAtual) {
        if (grafo[u][v] == 0) { // Se v não é adjacente a todos os vértices no clique
            return false;
        }
    }
    return true;
}

// Função recursiva para encontrar a maior clique com heurística
void BuscarCliqueMaxima(vector<int>& cliqueAtual, const vector<int>& ordenados, int indice) {
    // Atualiza a melhor clique caso a atual seja maior
    if (cliqueAtual.size() > melhorClique.size()) {
        melhorClique = cliqueAtual;
    }

    // Tenta adicionar novos vértices à clique atual
    for (int i = indice; i < ordenados.size(); i++) {
        int v = ordenados[i];
        if (PodeAdicionar(cliqueAtual, v)) { // Checa se é possível adicionar o vértice
            cliqueAtual.push_back(v);
            BuscarCliqueMaxima(cliqueAtual, ordenados, i + 1); // Busca a partir do próximo vértice
            cliqueAtual.pop_back(); // Remove o vértice após a chamada recursiva
        }
    }
}

int main() {
    string nomeArquivo = "grafo.txt"; // Nome do arquivo de entrada

    // Lê o grafo do arquivo
    LerGrafo(nomeArquivo);

    // Calcula o grau dos vértices
    vector<int> grau = CalcularGrau();

    // Ordena os vértices por grau em ordem decrescente
    vector<int> ordenados(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        ordenados[i] = i;
    }
    sort(ordenados.begin(), ordenados.end(), [&](int a, int b) {
        return grau[a] > grau[b];
    });

    // Início da medição do tempo
    auto inicio = high_resolution_clock::now();

    // Encontra a clique máxima usando busca exaustiva com heurística
    vector<int> cliqueAtual;
    BuscarCliqueMaxima(cliqueAtual, ordenados, 0);

    // Fim da medição do tempo
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<seconds>(fim - inicio);

    // Exibe o resultado
    cout << "Clique Máxima Encontrada: ";
    for (int v : melhorClique) {
        cout << (v + 1) << " "; // Ajuste para 1-based index
    }
    cout << endl;

    cout << "Tamanho da Clique Máxima: " << melhorClique.size() << endl;
    cout << "Tempo de Execução: " << duracao.count() << " segundos" << endl;

    return 0;
}
