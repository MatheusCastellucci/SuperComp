#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array_size = 100; // tamanho total do array
    int *array = NULL;
    int local_size = array_size / size;
    int *local_array = (int*) malloc(local_size * sizeof(int));

    if (rank == 0) {
        // Processo raiz inicializa o array com valores aleatórios
        array = (int*) malloc(array_size * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < array_size; i++) {
            array[i] = rand() % 100 + 1; // valores aleatórios entre 1 e 100
        }
    }

    // Distribui o array entre os processos
    MPI_Scatter(array, local_size, MPI_INT, local_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo calcula a média local
    double local_sum = 0.0;
    for (int i = 0; i < local_size; i++) {
        local_sum += local_array[i];
    }
    double local_mean = local_sum / local_size;

    // Cada processo calcula a variância local
    double local_variance_sum = 0.0;
    for (int i = 0; i < local_size; i++) {
        local_variance_sum += (local_array[i] - local_mean) * (local_array[i] - local_mean);
    }
    double local_variance = local_variance_sum / local_size;

    // Coleta as médias e variâncias locais no processo raiz
    double *local_means = NULL;
    double *local_variances = NULL;
    if (rank == 0) {
        local_means = (double*) malloc(size * sizeof(double));
        local_variances = (double*) malloc(size * sizeof(double));
    }
    MPI_Gather(&local_mean, 1, MPI_DOUBLE, local_means, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_variance, 1, MPI_DOUBLE, local_variances, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz calcula o desvio padrão global
    if (rank == 0) {
        double global_mean = 0.0;
        for (int i = 0; i < size; i++) {
            global_mean += local_means[i];
        }
        global_mean /= size;

        double global_variance = 0.0;
        for (int i = 0; i < size; i++) {
            global_variance += local_variances[i];
        }
        global_variance /= size;

        double global_std_dev = sqrt(global_variance);
        printf("Desvio padrão global: %f\n", global_std_dev);

        free(array);
        free(local_means);
        free(local_variances);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}