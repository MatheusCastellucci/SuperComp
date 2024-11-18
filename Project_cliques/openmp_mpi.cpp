#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

// Função para verificar se um vértice pode ser adicionado ao clique atual
bool PodeAdicionar(const vector<int>& cliqueAtual, int v, const vector<vector<int>>& grafo) {
    for (int u : cliqueAtual) {
        if (grafo[u][v] == 0) return false;
    }
    return true;
}

// Função para encontrar cliques maximais em uma parte do grafo
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

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<vector<int>> grafo;
    int numVertices;
    int verticesPorProcesso;

    if (rank == 0) {
        // O processo mestre lê o grafo completo
        string nomeArquivo = "grafo.txt";
        ifstream arquivo(nomeArquivo);

        int numArestas;
        arquivo >> numVertices >> numArestas;

        grafo.assign(numVertices, vector<int>(numVertices, 0));

        for (int i = 0; i < numArestas; ++i) {
            int u, v;
            arquivo >> u >> v;
            grafo[u - 1][v - 1] = 1;
            grafo[v - 1][u - 1] = 1;
        }

        arquivo.close();

        // Determina quantos vértices cada processo vai processar
        verticesPorProcesso = (numVertices + size - 1) / size;
    }

    // Broadcast do número de vértices e vértices por processo
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&verticesPorProcesso, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo aloca espaço para sua parte do grafo
    vector<vector<int>> subGrafo(verticesPorProcesso, vector<int>(numVertices, 0));

    // Scatter dos dados do grafo para todos os processos
    vector<int> bufferGrafo;
    if (rank == 0) {
        for (const auto& linha : grafo) {
            bufferGrafo.insert(bufferGrafo.end(), linha.begin(), linha.end());
        }
    }

    vector<int> bufferSubGrafo(verticesPorProcesso * numVertices, 0);
    MPI_Scatter(bufferGrafo.data(), verticesPorProcesso * numVertices, MPI_INT,
                bufferSubGrafo.data(), verticesPorProcesso * numVertices, MPI_INT, 0, MPI_COMM_WORLD);

    // Reconstrução do subgrafo local
    for (int i = 0; i < verticesPorProcesso; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            subGrafo[i][j] = bufferSubGrafo[i * numVertices + j];
        }
    }

    // Medir o tempo de execução
    double inicio = MPI_Wtime();

    // Cada processo encontra cliques maximais na sua parte do grafo
    vector<vector<int>> cliquesMaximais = EncontrarCliquesMaximais(subGrafo, verticesPorProcesso);

    // Cada processo encontra sua maior clique
    vector<int> cliqueMaximaLocal = EncontrarCliqueMaxima(cliquesMaximais);

    // O processo mestre reúne as cliques máximas de todos os processos
    vector<int> cliqueMaximaGlobal(cliqueMaximaLocal.size());
    MPI_Reduce(cliqueMaximaLocal.data(), cliqueMaximaGlobal.data(), cliqueMaximaLocal.size(),
               MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    double fim = MPI_Wtime();

    // O processo mestre exibe o resultado
    if (rank == 0) {
        cout << "Clique Máxima Encontrada: ";
        for (int v : cliqueMaximaGlobal) {
            cout << (v + 1) << " "; // Ajuste para 1-based index
        }
        cout << endl;
        cout << "Tamanho da Clique Máxima: " << cliqueMaximaGlobal.size() << endl;
        cout << "Tempo de Execução: " << (fim - inicio) << " segundos" << endl;
    }

    MPI_Finalize();
    return 0;
}
