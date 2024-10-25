#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_iter = 0;

    if (rank == 0) {
        // Processo raiz define a configuração inicial
        num_iter = 100; // número de iterações para o cálculo
        printf("Processo raiz define num_iter = %d\n", num_iter);
    }

    // Broadcast da configuração para todos os processos
    MPI_Bcast(&num_iter, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo executa o cálculo
    int local_sum = 0;
    for (int i = 0; i <= num_iter; i++) {
        local_sum += i;
    }

    // Cada processo exibe seu resultado local
    printf("Processo %d calculou a soma de 0 até %d: %d\n", rank, num_iter, local_sum);

    MPI_Finalize();
    return 0;
}
