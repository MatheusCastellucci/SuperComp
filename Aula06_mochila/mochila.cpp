#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;

// Estrutura para armazenar peso e valor de um item
struct Item {
    int peso;
    int valor;
};

// Função recursiva para calcular o valor máximo que pode ser colocado na mochila
int mochilaRecursiva(int capacidade, vector<Item>& itens, int n) {
    // Caso base: sem itens ou capacidade zero
    if (n == 0 || capacidade == 0) 
        return 0;

    // Se o peso do n-ésimo item é mais que a capacidade da mochila, ele não pode ser incluído
    if (itens[n-1].peso > capacidade)
        return mochilaRecursiva(capacidade, itens, n-1);

    // Retorna o máximo entre incluir ou não o n-ésimo item
    return max(itens[n-1].valor + mochilaRecursiva(capacidade - itens[n-1].peso, itens, n-1),
               mochilaRecursiva(capacidade, itens, n-1));
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
    vector<string> arquivosEntrada = {"in1.txt", "in2.txt", "in3.txt", "in4.txt"};

    for (const auto& nomeArquivo : arquivosEntrada) {
        int capacidade;
        vector<Item> itens;

        lerEntrada("/mnt/data/" + nomeArquivo, capacidade, itens);

        auto inicio = chrono::high_resolution_clock::now();
        int valorMaximo = mochilaRecursiva(capacidade, itens, itens.size());
        auto fim = chrono::high_resolution_clock::now();
        chrono::duration<double> duracao = fim - inicio;

        cout << "Arquivo: " << nomeArquivo << endl;
        cout << "Valor máximo na mochila: " << valorMaximo << endl;
        cout << "Tempo de execução: " << duracao.count() << " segundos" << endl;
        cout << "---------------------------" << endl;
    }

    return 0;
}
