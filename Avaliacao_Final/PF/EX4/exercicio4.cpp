#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <cstdlib>

const int N = 1000;  // Dimensão da matriz

std::vector<double> calculaMedia(const std::vector<double>& matrix, int linhasProcesso, int cols) {
    std::vector<double> mediaLinha(linhasProcesso);
    #pragma omp parallel for
    for (int i = 0; i < linhasProcesso; ++i) {
        double soma = 0.0;
        for (int j = 0; j < cols; ++j) {
            soma += matrix[i * cols + j];
        }
        mediaLinha[i] = soma / cols;
    }
    return mediaLinha;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double startTime = MPI_Wtime();

    std::vector<double> matrix;
    if (rank == 0) {
        matrix.resize(N * N);
        for (int i = 0; i < N * N; ++i) {
            matrix[i] = rand() % 100;
        }
    }

    int linhasProcesso = N / size;
    std::vector<double> localMatrix(linhasProcesso * N);
    MPI_Scatter(matrix.data(), linhasProcesso * N, MPI_DOUBLE, localMatrix.data(), linhasProcesso * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> mediaLocal = calculaMedia(localMatrix, linhasProcesso, N);

    std::vector<double> todasMedias;
    if (rank == 0) {
        todasMedias.resize(N);
    }
    MPI_Gather(mediaLocal.data(), linhasProcesso, MPI_DOUBLE, todasMedias.data(), linhasProcesso, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double totalSum = 0.0;
        for (double avg : todasMedias) {
            totalSum += avg;
        }
        double mediaTotal = totalSum / N;
        std::cout << "Média total de todas as linhas: " << mediaTotal << std::endl;
    }

    double endTime = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Tempo de processamento: " << endTime - startTime << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}