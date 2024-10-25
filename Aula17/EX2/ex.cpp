#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Certifique-se de que o número de processos seja maior que 2
    if (size < 3) {
        if (rank == 0) {
            printf("O número de processos deve ser maior que 2.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Mensagem que será enviada
    int message = rank; // cada processo envia seu rank como mensagem
    int recv_message;

    // O processo 0 começa enviando a mensagem
    if (rank == 0) {
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Processo %d enviou mensagem %d para o processo 1\n", rank, message);

        // O processo 0 recebe a mensagem do último processo
        MPI_Recv(&recv_message, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu mensagem %d do processo %d\n", rank, recv_message, size - 1);

    } else {
        // Outros processos recebem a mensagem do processo anterior
        MPI_Recv(&recv_message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu mensagem %d do processo %d\n", rank, recv_message, rank - 1);

        // O último processo envia de volta para o processo 0
        if (rank == size - 1) {
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            printf("Processo %d enviou mensagem %d para o processo 0\n", rank, message);
        } else {
            // Outros processos enviam a mensagem para o próximo processo
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            printf("Processo %d enviou mensagem %d para o processo %d\n", rank, message, rank + 1);
        }
    }

    MPI_Finalize();
    return 0;
}
