#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <chrono>
#include <omp.h>

int main() {
    int N = 100000000;
    int pontosDentroDoCirculo = 0;

    // Captura o tempo de início
    auto inicio = std::chrono::high_resolution_clock::now();

    // Paralelização com OpenMP e gerador de números aleatórios por thread
    #pragma omp parallel
    {
        // Obter o número da thread
        int thread_id = omp_get_thread_num();

        // Criar gerador de números aleatórios por thread com semente baseada no id da thread
        std::mt19937 gerador(thread_id + std::time(nullptr));
        std::uniform_real_distribution<double> distribuicao(0.0, 1.0);

        // Variável local para a contagem de pontos dentro do círculo para cada thread
        int pontosLocaisDentroDoCirculo = 0;

        // Paralelizar o sorteio de pontos
        #pragma omp for
        for (int i = 0; i < N; ++i) {
            double x = distribuicao(gerador);
            double y = distribuicao(gerador);

            if (x * x + y * y <= 1) {
                ++pontosLocaisDentroDoCirculo;
            }
        }

        // Redução da contagem total de pontos dentro do círculo
        #pragma omp atomic
        pontosDentroDoCirculo += pontosLocaisDentroDoCirculo;
    }

    // Estimar o valor de pi
    double pi = 4.0 * pontosDentroDoCirculo / N;

    // Captura o tempo de término
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Valor estimado de pi: " << pi << std::endl;
    std::cout << "Tempo de execução (paralelo com gerador por thread): " << duracao.count() << " segundos" << std::endl;

    return 0;
}
