#include <iostream>
#include <algorithm>  // Para std::copy

class Vector {
public:
    // Construtor que inicializa um vetor com um tamanho especificado
    Vector(int tamanho) : tam(0), capacidade(tamanho) {
        dados = new int[capacidade];
    }

    // Destrutor que libera a memória alocada para o vetor
    ~Vector() {
        delete[] dados;
    }

    // Inicializa o vetor com um valor específico
    void inicializa(int valor) {
        std::fill(dados, dados + capacidade, valor);
        tam = capacidade;
    }

    // Retorna o valor do elemento no índice especificado
    int get(int index) const {
        if (index >= 0 && index < tam) {
            return dados[index];
        }
        throw std::out_of_range("Índice fora dos limites.");
    }

    // Define o valor do elemento no índice especificado
    void set(int index, int valor) {
        if (index >= 0 && index < tam) {
            dados[index] = valor;
        } else {
            throw std::out_of_range("Índice fora dos limites.");
        }
    }

    // Insere um elemento no índice especificado
    void inserir(int index, int valor) {
        if (index < 0 || index > tam) {
            throw std::out_of_range("Índice fora dos limites.");
        }
        if (tam == capacidade) {
            redimensiona(capacidade * 2);
        }
        std::move_backward(dados + index, dados + tam, dados + tam + 1);
        dados[index] = valor;
        ++tam;
    }

    // Remove o elemento no índice especificado
    void remover(int index) {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Índice fora dos limites.");
        }
        std::move(dados + index + 1, dados + tam, dados + index);
        --tam;
    }

    // Imprime todos os elementos do vetor
    void imprime() const {
        for (int i = 0; i < tam; ++i) {
            std::cout << dados[i] << " ";
        }
        std::cout << std::endl;
    }

    // Multiplica todos os elementos do vetor por um escalar
    void multiplicarPorEscalar(int escalar) {
        for (int i = 0; i < tam; ++i) {
            dados[i] *= escalar;
        }
    }

private:
    int* dados;       // Ponteiro para os dados do vetor
    int tam;          // Número de elementos atualmente no vetor
    int capacidade;   // Capacidade total do vetor

    // Redimensiona o vetor para uma nova capacidade
    void redimensiona(int novaCapacidade) {
        int* novosDados = new int[novaCapacidade];
        std::copy(dados, dados + tam, novosDados);
        delete[] dados;
        dados = novosDados;
        capacidade = novaCapacidade;
    }
};

int main() {
    Vector vec(5);
    vec.inicializa(1); // Inicializa o vetor com o valor 1
    vec.imprime(); // Imprime: 1 1 1 1 1

    vec.inserir(2, 10); // Insere o valor 10 na posição 2
    vec.imprime(); // Imprime: 1 1 10 1 1 1

    vec.remover(3); // Remove o valor na posição 3
    vec.imprime(); // Imprime: 1 1 10 1 1

    vec.multiplicarPorEscalar(5); // Multiplica todos os elementos por 5
    vec.imprime(); // Imprime: 5 5 50 5 5

    return 0;
}
