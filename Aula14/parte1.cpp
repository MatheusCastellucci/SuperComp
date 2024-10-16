#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

int main() {
    int N = 100000000;
    int pontosDentroDoCirculo = 0;

    std::srand(std::time(nullptr));

    // Captura o tempo de início
    auto inicio = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        double x = static_cast<double>(std::rand()) / RAND_MAX;
        double y = static_cast<double>(std::rand()) / RAND_MAX;

        if (x * x + y * y <= 1) {
            ++pontosDentroDoCirculo;
        }
    }

    double pi = 4.0 * pontosDentroDoCirculo / N;

    // Captura o tempo de término
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Valor estimado de pi: " << pi << std::endl;
    std::cout << "Tempo de execução: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
