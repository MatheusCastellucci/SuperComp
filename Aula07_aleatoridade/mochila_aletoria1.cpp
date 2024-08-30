#include <iostream>  // Biblioteca padrão para entrada e saída de dados
#include <vector>    // Biblioteca para uso de vetores dinâmicos
#include <fstream>   // Biblioteca para manipulação de arquivos
#include <algorithm> // Biblioteca para funções padrão, como 'max'
#include <random>    // Biblioteca para geração de números aleatórios

using namespace std; // Usar o namespace padrão para evitar escrever std:: repetidamente

// Estrutura para armazenar peso e valor de um item
struct Item {
    int peso;   // Peso do item
    int valor;  // Valor do item
};

// Função que implementa a heurística aleatória para o problema da mochila
long long mochilaHeuristica(int capacidade, vector<Item>& itens) {
    // Embaralhar o vetor de itens
    random_device rd;
    mt19937 g(rd());
    shuffle(itens.begin(), itens.end(), g);
    
    long long valorTotal = 0;
    int pesoAtual = 0;
    
    // Percorrer o vetor embaralhado e adicionar itens à mochila se couberem
    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorTotal += item.valor;
        } else {
            break; // Parar quando a mochila estiver cheia
        }
    }
    
    return valorTotal;
}

// Função para ler os dados de entrada de um arquivo
void lerEntrada(const string& nomeArquivo, vector<Item>& itens, int& capacidade) {
    ifstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        int n;
        arquivo >> n >> capacidade;
        itens.resize(n);
        for (int i = 0; i < n; i++) {
            arquivo >> itens[i].peso >> itens[i].valor;
        }
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo!" << endl;
    }
}

int main() {
    vector<Item> itens;
    int capacidade;
    
    // Ler a entrada de um arquivo (modifique o nome do arquivo conforme necessário)
    lerEntrada("entrada.txt", itens, capacidade);
    
    // Exemplo de uso da função de heurística
    long long valorHeuristica = mochilaHeuristica(capacidade, itens);
    cout << "Valor obtido pela heuristica: " << valorHeuristica << endl;
    
    return 0;
}