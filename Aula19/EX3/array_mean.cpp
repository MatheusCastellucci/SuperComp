#include <iostream>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000;  // Tamanho do vetor
    int chunk_size = N / size;
    double *data = nullptr;

    // Início da medição de tempo
    double start_time = MPI_Wtime();

    // Inicialização do vetor no processo 0
    if (rank == 0) {
        data = new double[N];
        for (int i = 0; i < N; i++) {
            data[i] = i + 1;  // Inicializando o vetor com valores de exemplo
        }
    }

    // Alocar espaço para o vetor local em cada processo
    double *local_data = new double[chunk_size];

    // Distribuir os dados para os processos
    MPI_Scatter(data, chunk_size, MPI_DOUBLE, local_data, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Cálculo da soma parcial com OpenMP
    double local_sum = 0.0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < chunk_size; i++) {
        local_sum += local_data[i];
    }

    // Coletar as somas parciais no processo 0
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Cálculo da média no processo 0
    if (rank == 0) {
        double average = total_sum / N;
        std::cout << "Média dos elementos do vetor: " << average << std::endl;
    }

    // Fim da medição de tempo
    double end_time = MPI_Wtime();

    // Exibir o tempo de execução no processo 0
    if (rank == 0) {
        std::cout << "Tempo de execução: " << end_time - start_time << " segundos" << std::endl;
    }

    // Liberar memória
    if (rank == 0) {
        delete[] data;
    }
    delete[] local_data;

    MPI_Finalize();
    return 0;
}
