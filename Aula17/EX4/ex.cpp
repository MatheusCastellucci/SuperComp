#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Processo 0 enviando mensagens exclusivas para todos os outros processos
        for (int i = 1; i < size; i++) {
            char message[30];
            sprintf(message, "Mensagem exclusiva para %d", i);
            MPI_Send(message, 30, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            printf("Processo 0 enviou: %s para o processo %d\n", message, i);
        }
    } else {
        // Outros processos recebem a mensagem e exibem
        char recv_message[30];
        MPI_Recv(recv_message, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu: %s\n", rank, recv_message);
    }

    MPI_Finalize();
    return 0;
}
