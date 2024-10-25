#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Processo 0 envia "Olá" para o processo 1
        char msg[] = "Olá";
        MPI_Send(msg, 4, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        printf("Processo 0 enviou: %s\n", msg);

        // Processo 0 recebe a resposta "Oi" do processo 1
        char recv_msg[3];
        MPI_Recv(recv_msg, 3, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo 0 recebeu: %s\n", recv_msg);

    } else if (rank == 1) {
        // Processo 1 recebe "Olá" do processo 0
        char recv_msg[4];
        MPI_Recv(recv_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo 1 recebeu: %s\n", recv_msg);

        // Processo 1 envia "Oi" de volta para o processo 0
        char msg[] = "Oi";
        MPI_Send(msg, 3, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        printf("Processo 1 enviou: %s\n", msg);
    }

    MPI_Finalize();
    return 0;
}
