#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message;

    // O processo 0 inicia a comunicação
    if (rank == 0) {
        message = 100; // Mensagem inicial que será passada
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Processo 0 enviou a mensagem %d para o processo 1\n", message);
    } else {
        // Todos os outros processos (exceto o último) recebem e repassam a mensagem
        MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu a mensagem %d do processo %d\n", rank, message, rank - 1);

        // Se não for o último processo, envia a mensagem para o próximo
        if (rank < size - 1) {
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            printf("Processo %d enviou a mensagem %d para o processo %d\n", rank, message, rank + 1);
        }
    }

    MPI_Finalize();
    return 0;
}