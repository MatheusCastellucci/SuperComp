#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char message[30];

    if (rank == 0) {
        // Processo 0 alterna mensagens com cada processo
        for (int i = 1; i < size; i++) {
            // Enviando mensagem para o processo i
            sprintf(message, "Mensagem para o processo %d", i);
            MPI_Send(message, 30, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            printf("Processo 0 enviou: %s\n", message);

            // Recebendo resposta do processo i
            MPI_Recv(message, 30, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Processo 0 recebeu resposta de %d: %s\n", i, message);
        }
    } else {
        // Todos os outros processos
        // Recebem a mensagem do processo 0
        MPI_Recv(message, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu: %s\n", rank, message);

        // Respondendo ao processo 0
        sprintf(message, "Resposta do processo %d", rank);
        MPI_Send(message, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        printf("Processo %d enviou resposta para o processo 0\n", rank);
    }

    MPI_Finalize();
    return 0;
}
