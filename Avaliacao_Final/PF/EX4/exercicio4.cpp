#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <cstdlib>

const int N = 1000;  // Dimensão da matriz

std::vector<double> calculaMediaLinha(const std::vector<std::vector<double>>& matrix, int startRow, int endRow) {
    std::vector<double> mediaLinha(endRow - startRow);
    #pragma omp parallel for
    for (int i = startRow; i < endRow; ++i) {
        double sum = 0.0;
        for (int j = 0; j < N; ++j) {
            sum += matrix[i][j];
        }
        mediaLinha[i - startRow] = sum / N;
    }
    return mediaLinha;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> matrix;
    if (rank == 0) {
        matrix.resize(N, std::vector<double>(N));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = rand() % 100;
            }
        }
    }

    int linhasPorProcesso = N / size;
    std::vector<std::vector<double>> localMatrix(linhasPorProcesso, std::vector<double>(N));
    MPI_Scatter(matrix.data()->data(), linhasPorProcesso * N, MPI_DOUBLE, localMatrix.data()->data(), linhasPorProcesso * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> localAverages = calculaMediaLinha(localMatrix, 0, linhasPorProcesso);

    std::vector<double> allAverages;
    if (rank == 0) {
        allAverages.resize(N);
    }
    MPI_Gather(localAverages.data(), linhasPorProcesso, MPI_DOUBLE, allAverages.data(), linhasPorProcesso, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double totalSum = 0.0;
        for (double avg : allAverages) {
            totalSum += avg;
        }
        double totalAverage = totalSum / N;
        std::cout << "Média total de todas as linhas: " << totalAverage << std::endl;
    }

    MPI_Finalize();
    return 0;
}