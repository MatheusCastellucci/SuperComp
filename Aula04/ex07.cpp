#include <iostream> // Inclui a biblioteca de entrada e saída padrão

void bubbleSort(int arr[], int n) {
    bool trocado;
    for (int i = 0; i < n - 1; ++i) {
        trocado = false;
        // Percorre o vetor da posição 0 até n-i-1
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Troca os elementos se estiverem na ordem errada
                std::swap(arr[j], arr[j + 1]);
                trocado = true;
            }
        }
        // Se não houve troca, o vetor já está ordenado
        if (!trocado) {
            break;
        }
    }
}

int main() {
    const int tamanho = 10; // Define o tamanho do vetor
    int numeros[tamanho];   // Declara um vetor para armazenar os números

    // Pede ao usuário para inserir 10 números
    std::cout << "Digite 10 numeros:" << std::endl;
    for (int i = 0; i < tamanho; ++i) {
        std::cout << "Numero " << (i + 1) << ": ";
        std::cin >> numeros[i]; // Lê o número e armazena no vetor
    }

    // Ordena o vetor usando Bubble Sort
    bubbleSort(numeros, tamanho);

    // Exibe o vetor ordenado
    std::cout << "Numeros em ordem crescente:" << std::endl;
    for (int i = 0; i < tamanho; ++i) {
        std::cout << numeros[i] << " ";
    }
    std::cout << std::endl;

    return 0; // Indica que o programa terminou com sucesso
}
