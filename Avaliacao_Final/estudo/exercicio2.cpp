#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  // Inicializa o MPI

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // Identifica o rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Identifica o número total de processos

    const int N = 100000;  // Tamanho do vetor
    std::vector<int> vetor;

    if (world_rank == 0) {
        // Inicializa o vetor apenas no processo raiz
        vetor.resize(N);
        for (int i = 0; i < N; i++) {
            vetor[i] = i + 1; // Vetor com valores de 1 a N
        }
    }

    // Divide o tamanho do vetor entre os processos
    int local_size = N / world_size;
    std::vector<int> local_vetor(local_size);

    // Distribui partes do vetor para todos os processos
    MPI_Scatter(vetor.data(), local_size, MPI_INT,
                local_vetor.data(), local_size, MPI_INT,
                0, MPI_COMM_WORLD);

    // Cálculo da soma dos quadrados no subvetor local usando OpenMP
    long long local_sum = 0;
#pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < local_size; i++) {
        local_sum += std::pow(local_vetor[i], 2);
    }

    // Coleta os resultados parciais no processo raiz
    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // O processo raiz exibe o resultado
    if (world_rank == 0) {
        std::cout << "Soma total dos quadrados: " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}


// mpicxx -fopenmp -o exercicio2 exercicio2.cpp
// mpirun -np 4 ./exercicio2
