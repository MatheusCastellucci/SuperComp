
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
                cout << "Adicionando item de peso " << itens[i].peso << " e valor " << itens[i].valor << endl;
            } else {
                cout << "Ignorando item de peso " << itens[i].peso << " por exceder a capacidade" << endl;
            }
        } else {
            cout << "Ignorando item de peso " << itens[i].peso << " e valor " << itens[i].valor << " com base na probabilidade" << endl;
        }
    }

    cout << "Peso total na mochila: " << pesoAtual << endl;
    cout << "Valor total na mochila: " << valorTotal << endl;

    return valorTotal;
}

int main() {
    string filename;
    cout << "Digite o nome do arquivo de entrada: ";
    cin >> filename;

    ifstream infile(filename);

    if (!infile) {
        cerr << "Erro ao abrir o arquivo " << filename << endl;
        return 1;
    }

    int capacidade, n;

    infile >> capacidade >> n;

    vector<Item> itens(n);

    // Leitura dos itens (peso e valor)
    for (int i = 0; i < n; i++) {
        infile >> itens[i].peso >> itens[i].valor;
    }

    infile.close();

    // Início da medição do tempo
    auto start = high_resolution_clock::now();

    // Aplicação da seleção aleatória baseada em probabilidade
    int valorMaximo = mochilaProbabilidade(capacidade, itens, n);

    // Fim da medição do tempo
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Exibir o valor total e o tempo de execução
    cout << "Valor total na mochila: " << valorMaximo << endl;
    cout << "Tempo de execução: " << duration.count() << " microsegundos" << endl;

    return 0;
}
