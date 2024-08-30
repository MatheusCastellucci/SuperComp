#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// Estrutura para armazenar peso e valor de um item
struct Item {
    int peso;
    int valor;
};

// Função que implementa a heurística aleatória para o problema da mochila
long long mochilaHeuristica(int capacidade, vector<Item>& itens) {
    random_device rd;
    mt19937 g(rd());
    shuffle(itens.begin(), itens.end(), g);
    
    long long valorTotal = 0;
    int pesoAtual = 0;
    
    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorTotal += item.valor;
        } else {
            break;
        }
    }
    
    return valorTotal;
}

// Função para calcular o peso correspondente ao valor máximo na heurística
int pesoHeuristica(int capacidade, vector<Item>& itens) {
    random_device rd;
    mt19937 g(rd());
    shuffle(itens.begin(), itens.end(), g);

    int pesoAtual = 0;
    long long valorTotal = 0;

    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorTotal += item.valor;
        } else {
            break;
        }
    }

    return pesoAtual;
}

// Função para ler os dados de entrada de um arquivo
void lerEntrada(const string& nomeArquivo, int& capacidade, vector<Item>& itens) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        exit(1);
    }
    int n;
    arquivo >> n >> capacidade;
    itens.resize(n);
    for (int i = 0; i < n; ++i) {
        arquivo >> itens[i].peso >> itens[i].valor;
    }
    arquivo.close();
}

int main() {
    vector<string> arquivosEntrada = {"Entrada_1.txt", "Entrada_2.txt", "Entrada_3.txt", "Entrada_4.txt"};

    for (const auto& nomeArquivo : arquivosEntrada) {
        int capacidade;
        vector<Item> itens;

        lerEntrada(nomeArquivo, capacidade, itens);

        auto inicio = chrono::high_resolution_clock::now();
        long long valorMaximo = mochilaHeuristica(capacidade, itens);
        int pesoOcupado = pesoHeuristica(capacidade, itens);
        auto fim = chrono::high_resolution_clock::now();
        chrono::duration<double> duracao = fim - inicio;

        cout << "Arquivo: " << nomeArquivo << endl;
        cout << "Peso ocupado: " << pesoOcupado << " Kg" << endl;
        cout << "Valor máximo na mochila: " << valorMaximo << " dinheiros" << endl;
        cout << "Tempo de execução: " << duracao.count() << " segundos" << endl;
        cout << "---------------------------" << endl;
    }

    return 0;
}
