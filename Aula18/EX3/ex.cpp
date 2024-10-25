#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array_size = 50; // tamanho total do array
    int *array = NULL;
    int local_size = array_size / size;
    int *local_array = (int*) malloc(local_size * sizeof(int));

    int max_value = 0;

    if (rank == 0) {
        // Processo raiz inicializa o array com valores aleatórios
        array = (int*) malloc(array_size * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < array_size; i++) {
            array[i] = rand() % 50 + 1; // valores aleatórios entre 1 e 100
            if (array[i] > max_value) {
                max_value = array[i];
            }
        }
        printf("Valor máximo encontrado pelo processo raiz: %d\n", max_value);
    }

    // Broadcast do valor máximo para todos os processos
    MPI_Bcast(&max_value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Distribui o array entre os processos
    MPI_Scatter(array, local_size, MPI_INT, local_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo normaliza sua parte do array
    for (int i = 0; i < local_size; i++) {
        local_array[i] = (double)local_array[i] / max_value;
    }

    // Reúne os arrays normalizados no processo raiz
    if (rank == 0) {
        int *normalized_array = (int*) malloc(array_size * sizeof(int));
        MPI_Gather(local_array, local_size, MPI_INT, normalized_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

        // Exibe o array normalizado
        printf("Array normalizado:\n");
        for (int i = 0; i < array_size; i++) {
            printf("%f ", (double)normalized_array[i]);
        }
        printf("\n");

        free(array);
        free(normalized_array);
    } else {
        MPI_Gather(local_array, local_size, MPI_INT, NULL, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
