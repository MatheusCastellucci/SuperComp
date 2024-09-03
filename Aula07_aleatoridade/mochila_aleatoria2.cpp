
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Estrutura para armazenar peso e valor de um item
struct Item {
    int peso;
    int valor;
};

// Função para preencher a mochila usando a seleção aleatória baseada em probabilidade
int mochilaProbabilidade(int capacidade, vector<Item>& itens, int n, double limiar = 0.5) {
    int valorTotal = 0;
    int pesoAtual = 0;

    // Inicializa o gerador de números aleatórios
    default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> distribution(0.0, 1.0);

    // Percorrer o vetor de itens
    for (int i = 0; i < n; i++) {
        double probabilidade = distribution(generator);
        cout << "Probabilidade gerada para o item " << i+1 << ": " << probabilidade << endl;
        if (probabilidade > limiar) {
            if (pesoAtual + itens[i].peso <= capacidade) {
                pesoAtual += itens[i].peso;
                valorTotal += itens[i].valor;
            } else {
                cout << "Ignorando item de peso " << itens[i].peso << " e valor " << itens[i].valor << " devido à capacidade excedida" << endl;
            }
        } else {
            cout << "Ignorando item de peso " << itens[i].peso << " e valor " << itens[i].valor << " com base na probabilidade" << endl;
        }
    }

    cout << "Peso total na mochila: " << pesoAtual << endl;
    cout << "Valor total na mochila: " << valorTotal << endl;

    return valorTotal;
}

void lerEntrada(const string& nomeArquivo, int& capacidade, vector<Item>& itens) {
    ifstream infile(nomeArquivo);

    if (!infile) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        exit(1);
    }

    int n;
    infile >> capacidade >> n;
    itens.resize(n);

    for (int i = 0; i < n; i++) {
        infile >> itens[i].peso >> itens[i].valor;
    }

    infile.close();
}

int main() {
    vector<string> arquivosEntrada = {"Entrada_1.txt", "Entrada_2.txt", "Entrada_3.txt", "Entrada_4.txt"};

    for (const auto& nomeArquivo : arquivosEntrada) {
        int capacidade;
        vector<Item> itens;
        
        lerEntrada(nomeArquivo, capacidade, itens);

        auto inicio = chrono::high_resolution_clock::now();
        long long valorMaximo = mochilaProbabilidade(capacidade, itens, itens.size());
        auto fim = chrono::high_resolution_clock::now();
        chrono::duration<double> duracao = fim - inicio;

        cout << "Arquivo: " << nomeArquivo << endl;
        cout << "Valor total na mochila: " << valorMaximo << " dinheiros" << endl;
        cout << "Tempo de execução: " << duracao.count() << " segundos" << endl;
        cout << "----------------------------" << endl;
    }

    return 0;
}
