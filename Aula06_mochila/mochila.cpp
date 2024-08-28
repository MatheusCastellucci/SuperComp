#include <iostream>  // Biblioteca padrão para entrada e saída de dados
#include <vector>    // Biblioteca para uso de vetores dinâmicos
#include <fstream>   // Biblioteca para manipulação de arquivos
#include <algorithm> // Biblioteca para funções padrão, como 'max'

using namespace std; // Usar o namespace padrão para evitar escrever std:: repetidamente

// Estrutura para armazenar peso e valor de um item
struct Item {
    int peso;   // Peso do item
    int valor;  // Valor do item
};

// Definição da tabela de memoização
const int maxn = 110;        // Número máximo de itens (ajuste conforme necessário)
const int maxp = 1e5 + 10;   // Capacidade máxima da mochila (ajuste conforme necessário)

long long dp[maxn][maxp];    // Tabela para armazenar o valor máximo calculado para cada subproblema
bool vis[maxn][maxp];        // Tabela para verificar se um subproblema já foi calculado

// Função recursiva com memoização para resolver o problema da mochila
long long mochilaDinamica(int i, int capacidade, const vector<Item>& itens, int n) {
    if (i == n) return 0; // Caso base: se todos os itens foram considerados, retorna 0
    if (vis[i][capacidade]) return dp[i][capacidade]; // Se o subproblema já foi resolvido, retorna o valor armazenado
    vis[i][capacidade] = true; // Marca o subproblema atual como resolvido

    // Primeira possibilidade: não incluir o item atual
    dp[i][capacidade] = mochilaDinamica(i + 1, capacidade, itens, n);

    // Segunda possibilidade: incluir o item atual (se couber na mochila)
    if (capacidade >= itens[i].peso) {
        dp[i][capacidade] = max(dp[i][capacidade],
                                mochilaDinamica(i + 1, capacidade - itens[i].peso, itens, n) + itens[i].valor);
        // Atualiza o valor máximo considerando a inclusão do item atual
    }

    return dp[i][capacidade]; // Retorna o valor máximo calculado para o subproblema atual
}

// Função para ler os dados de entrada de um arquivo
void lerEntrada(const string& nomeArquivo, int& capacidade, vector<Item>& itens) {
    ifstream arquivo(nomeArquivo); // Abre o arquivo de entrada
    if (!arquivo) { // Verifica se o arquivo foi aberto corretamente
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl; // Exibe mensagem de erro se não conseguir abrir
        exit(1); // Sai do programa com código de erro
    }
    int n; // Variável para armazenar o número de itens
    arquivo >> n >> capacidade; // Lê o número de itens e a capacidade da mochila do arquivo
    itens.resize(n); // Redimensiona o vetor de itens para armazenar os 'n' itens
    for (int i = 0; i < n; ++i) { // Loop para ler cada item do arquivo
        arquivo >> itens[i].peso >> itens[i].valor; // Lê o peso e o valor de cada item
    }
    arquivo.close(); // Fecha o arquivo após a leitura
}

int main() {
    vector<string> arquivosEntrada = {"in1.txt", "in2.txt", "in3.txt", "in4.txt"};
    // Vetor de strings contendo os nomes dos arquivos de entrada

    for (const auto& nomeArquivo : arquivosEntrada) { // Loop para processar cada arquivo de entrada
        int capacidade; // Variável para armazenar a capacidade da mochila
        vector<Item> itens; // Vetor para armazenar os itens

        // Inicializa a tabela de memoização e vetor de visitação
        fill(&dp[0][0], &dp[0][0] + sizeof(dp) / sizeof(dp[0][0]), 0);
        fill(&vis[0][0], &vis[0][0] + sizeof(vis) / sizeof(vis[0][0]), false);
        // A função fill preenche as tabelas dp e vis com zeros e falsos, respectivamente

        lerEntrada(nomeArquivo, capacidade, itens); // Lê os dados do arquivo atual

        int valorMaximo = mochilaDinamica(0, capacidade, itens, itens.size());
        // Chama a função para resolver o problema da mochila a partir do primeiro item com a capacidade total

        cout << "Arquivo: " << nomeArquivo << endl;
        cout << "Valor máximo na mochila: " << valorMaximo << endl;
        cout << "---------------------------" << endl;
        // Exibe o valor máximo encontrado para o arquivo de entrada atual
    }

    return 0; // Retorna 0 para indicar que o programa terminou corretamente
}
