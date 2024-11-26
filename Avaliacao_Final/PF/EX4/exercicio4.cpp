#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <cstdlib>

const int N = 1000;  // Dimensão da matriz

std::vector<double> calculateRowAverages(const std::vector<double>& matrix, int rowsPerProcess, int cols) {
    std::vector<double> rowAverages(rowsPerProcess);
    #pragma omp parallel for
    for (int i = 0; i < rowsPerProcess; ++i) {
        double sum = 0.0;
        for (int j = 0; j < cols; ++j) {
            sum += matrix[i * cols + j];
        }
        rowAverages[i] = sum / cols;
    }
    return rowAverages;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> matrix;
    if (rank == 0) {
        matrix.resize(N * N);
        for (int i = 0; i < N * N; ++i) {
            matrix[i] = rand() % 100;
        }
    }

    int rowsPerProcess = N / size;
    std::vector<double> localMatrix(rowsPerProcess * N);
    MPI_Scatter(matrix.data(), rowsPerProcess * N, MPI_DOUBLE, localMatrix.data(), rowsPerProcess * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> localAverages = calculateRowAverages(localMatrix, rowsPerProcess, N);

    std::vector<double> allAverages;
    if (rank == 0) {
        allAverages.resize(N);
    }
    MPI_Gather(localAverages.data(), rowsPerProcess, MPI_DOUBLE, allAverages.data(), rowsPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

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