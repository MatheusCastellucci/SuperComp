#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array_size = 100; // tamanho total do array
    int *array = NULL; // array que será inicializado pelo processo raiz
    int local_size = array_size / size; // tamanho da parte que cada processo irá receber
    int *local_array = (int*) malloc(local_size * sizeof(int)); // alocação para a parte local de cada processo

    if (rank == 0) {
        // Processo raiz inicializa o array com valores aleatórios
        array = (int*) malloc(array_size * sizeof(int));
        for (int i = 0; i < array_size; i++) {
            array[i] = rand() % 50; // valores aleatórios entre 0 e 50
        }
    }

    // Distribui o array entre todos os processos
    MPI_Scatter(array, local_size, MPI_INT, local_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo calcula a média local
    double local_sum = 0.0;
    for (int i = 0; i < local_size; i++) {
        local_sum += local_array[i];
    }
    double local_avg = local_sum / local_size;

    // Reúne as médias locais no processo raiz
    double *local_avgs = NULL;
    if (rank == 0) {
        local_avgs = (double*) malloc(size * sizeof(double));
    }
    MPI_Gather(&local_avg, 1, MPI_DOUBLE, local_avgs, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz calcula a média global
    if (rank == 0) {
        double global_sum = 0.0;
        for (int i = 0; i < size; i++) {
            global_sum += local_avgs[i];
        }
        double global_avg = global_sum / size;
        printf("A média global é: %f\n", global_avg);

        free(array); // libera o array inicializado pelo processo raiz
        free(local_avgs); // libera o array de médias locais
    }

    free(local_array); // libera o array local de cada processo
    MPI_Finalize();
    return 0;
}
