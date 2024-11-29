#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace std::chrono;

// Variáveis globais
vector<vector<int>> grafo;
vector<int> melhorClique;
int numVertices;
vector<int> ordemVertices; // Ordem dos vértices após aplicar a heurística

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

// Função para verificar se um conjunto de vértices forma uma clique
bool VerificaClique(const vector<int>& clique) {
    for (size_t i = 0; i < clique.size(); ++i) {
        for (size_t j = i + 1; j < clique.size(); ++j) {
            if (grafo[clique[i]][clique[j]] == 0) { // Se não estão conectados
                return false;
            }
        }
    }
    return true;
}

// Função para encontrar a maior clique usando força bruta com a heurística
void BuscarCliqueMaxima() {
    int totalCombinacoes = pow(2, numVertices); // 2^numVertices combinações possíveis

    for (int mascara = 0; mascara < totalCombinacoes; ++mascara) {
        vector<int> cliqueAtual;

        // Construir subconjunto correspondente à máscara usando a ordem ajustada
        for (int i = 0; i < numVertices; ++i) {
            if (mascara & (1 << i)) { // Se o i-ésimo bit da máscara está definido
                cliqueAtual.push_back(ordemVertices[i]);
            }
        }

        // Verificar se o subconjunto forma uma clique
        if (VerificaClique(cliqueAtual)) {
            if (cliqueAtual.size() > melhorClique.size()) {
                melhorClique = cliqueAtual; // Atualizar a melhor clique
            }
        }
    }
}

int main() {
    string nomeArquivo = "grafo.txt"; // Nome do arquivo de entrada

    // Lê o grafo do arquivo
    LerGrafo(nomeArquivo);

    // Calcular o grau de cada vértice
    vector<int> grau = CalcularGrau();

    // Ordenar os vértices por grau (maior para menor)
    ordemVertices.resize(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        ordemVertices[i] = i;
    }
    sort(ordemVertices.begin(), ordemVertices.end(), [&](int a, int b) {
        return grau[a] > grau[b];
    });

    // Início da medição do tempo
    auto inicio = high_resolution_clock::now();

    // Encontra a clique máxima usando força bruta com heurística
    BuscarCliqueMaxima();

    // Fim da medição do tempo
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio);

    // Exibe o resultado
    cout << "Clique Máxima Encontrada: ";
    for (int v : melhorClique) {
        cout << (v + 1) << " "; // Ajuste para 1-based index
    }
    cout << endl;

    cout << "Tamanho da Clique Máxima: " << melhorClique.size() << endl;
    cout << "Tempo de Execução: " << duracao.count() << " ms" << endl;

    return 0;
}
