#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <omp.h>

const int N = 500;  // Dimensão da matriz

void matrixMultiply(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B, std::vector<std::vector<double>>& C) {
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    // Inicializa as matrizes
    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    std::vector<std::vector<double>> C(N, std::vector<double>(N));

    // Atribui valores aleatórios a A e B
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    // Realiza a multiplicação de matrizes
    // Cronometrando a multiplicação
    double start_time = omp_get_wtime();
    matrixMultiply(A, B, C);
    double time_taken = omp_get_wtime() - start_time;
    std::cout << "Tempo gasto para a multiplicação de matrizes: " << time_taken << " segundos." << std::endl;

    // Opcionalmente, imprime a matriz C
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         std::cout << C[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}