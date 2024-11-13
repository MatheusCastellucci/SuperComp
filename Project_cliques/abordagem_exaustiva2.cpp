#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono> // Para medir o tempo

using namespace std;
using namespace std::chrono;

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

// Função para calcular o grau de cada vértice
vector<int> CalcularGrau(const vector<vector<int>>& grafo) {
    int numVertices = grafo.size();
    vector<int> grau(numVertices, 0);

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            grau[i] += grafo[i][j];
        }
    }

    return grau;
}

// Função para encontrar o clique máximo em um grafo com heurística de ordenação por grau
vector<int> EncontrarCliqueMaxima(const vector<vector<int>>& grafo, int numVertices) {
    vector<int> cliqueMaxima;      // Armazena o clique máximo encontrado
    vector<int> candidatos(numVertices); // Lista de candidatos

    // Preenche a lista de candidatos inicialmente com todos os vértices
    for (int i = 0; i < numVertices; i++) {
        candidatos[i] = i;
    }

    // Calcula o grau dos vértices e ordena os candidatos pelo grau em ordem decrescente
    vector<int> grau = CalcularGrau(grafo);
    sort(candidatos.begin(), candidatos.end(), [&](int a, int b) {
        return grau[a] > grau[b];
    });

    // Enquanto houver candidatos, tentamos adicionar ao clique
    while (!candidatos.empty()) {
        int v = candidatos.back(); // Seleciona o candidato com menor grau restante
        candidatos.pop_back();     // Remove o último candidato

        bool podeAdicionar = true;

        // Verifica se o nó selecionado é adjacente a todos os nós já no clique
        for (int u : cliqueMaxima) {
            if (grafo[u][v] == 0) { // Se não há conexão entre u e v
                podeAdicionar = false;
                break;
            }
        }

        // Se o nó pode ser adicionado ao clique máximo
        if (podeAdicionar) {
            cliqueMaxima.push_back(v); // Adiciona v ao clique máximo
            vector<int> novosCandidatos;

            // Filtra os candidatos restantes que são adjacentes a todos no clique
            for (int u : candidatos) {
                bool adjacenteATodos = true;
                for (int c : cliqueMaxima) {
                    if (grafo[u][c] == 0) { // Se u não é adjacente a algum no clique
                        adjacenteATodos = false;
                        break;
                    }
                }
                if (adjacenteATodos) {
                    novosCandidatos.push_back(u);
                }
            }

            candidatos = novosCandidatos; // Atualiza os candidatos
        }
    }

    return cliqueMaxima; // Retorna o clique máximo encontrado
}

int main() {
    int numVertices;
    string nomeArquivo = "grafo.txt"; // Nome do arquivo de entrada

    // Lê o grafo do arquivo
    vector<vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    // Medindo o tempo de execução da abordagem com heurística
    auto inicio = high_resolution_clock::now();
    vector<int> cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices);
    auto fim = high_resolution_clock::now();

    // Calcula o tempo de execução
    auto duracao = duration_cast<milliseconds>(fim - inicio);

    // Exibe o resultado
    cout << "Clique Máxima Encontrada (Abordagem com Heurística): ";
    for (int v : cliqueMaxima) {
        cout << v + 1 << " "; // Ajusta a indexação para iniciar em 1
    }
    cout << endl;

    cout << "Tempo de Execução: " << duracao.count() << " ms" << endl;

    return 0;
}
