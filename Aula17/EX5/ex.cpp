#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cada processo tem um valor exclusivo (por exemplo, o valor do rank)
    int value = rank + 1;  // Valores: 1, 2, 3, ..., size
    int received_sum = 0;  // Para armazenar a soma recebida

    if (rank == 0) {
        // Processo 0 inicia a soma com seu próprio valor
        int current_sum = value;
        MPI_Send(&current_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Processo %d iniciou com valor %d e enviou soma %d para o processo 1\n", rank, value, current_sum);
    } else {
        // Todos os outros processos recebem a soma do processo anterior
        MPI_Recv(&received_sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int current_sum = received_sum + value;
        printf("Processo %d recebeu soma %d do processo %d e somou seu valor %d, resultando em %d\n", rank, received_sum, rank - 1, value, current_sum);

        // Se não for o último processo, envia a nova soma para o próximo processo
        if (rank < size - 1) {
            MPI_Send(&current_sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            printf("Processo %d enviou a soma %d para o processo %d\n", rank, current_sum, rank + 1);
        } else {
            // Último processo exibe a soma total
            printf("Processo %d é o último e a soma total é %d\n", rank, current_sum);
        }
    }

    MPI_Finalize();
    return 0;
}
