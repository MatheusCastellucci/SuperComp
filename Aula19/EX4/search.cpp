#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <random>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000;  // Tamanho do vetor
    const int target = 50;  // Valor a ser buscado
    int chunk_size = N / size;
    int *data = nullptr;

    // Inicialização do vetor aleatório no processo 0
    if (rank == 0) {
        data = new int[N];
        
        // Configuração do gerador de números aleatórios
        std::random_device rd;
        std::mt19937 gen(rd());  // Mersenne Twister para melhor distribuição
        std::uniform_int_distribution<> dis(0, 99);  // Valores aleatórios entre 0 e 99

        for (int i = 0; i < N; i++) {
            data[i] = dis(gen);  // Gera um valor aleatório para cada posição
        }
    }

    // Alocar espaço para o vetor local em cada processo
    int *local_data = new int[chunk_size];

    // Distribuir os dados para os processos
    MPI_Scatter(data, chunk_size, MPI_INT, local_data, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Vetor para armazenar as posições encontradas localmente
    std::vector<int> local_positions;

    // Busca paralela com OpenMP
    #pragma omp parallel for
    for (int i = 0; i < chunk_size; i++) {
        if (local_data[i] == target) {
            #pragma omp critical
            local_positions.push_back(i + rank * chunk_size);  // Ajusta o índice para o índice global
        }
    }

    // Coleta das posições encontradas em cada processo no processo 0
    if (rank == 0) {
        // Receber tamanhos de posições encontradas de cada processo
        std::vector<int> all_positions;
        for (int pos : local_positions) {
            all_positions.push_back(pos);
        }
        for (int p = 1; p < size; p++) {
            int recv_size;
            MPI_Recv(&recv_size, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::vector<int> recv_positions(recv_size);
            MPI_Recv(recv_positions.data(), recv_size, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            all_positions.insert(all_positions.end(), recv_positions.begin(), recv_positions.end());
        }

        // Imprimir todas as posições encontradas
        std::cout << "Posições do valor " << target << " encontradas: ";
        for (int pos : all_positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;

    } else {
        // Enviar o tamanho do vetor de posições e o vetor de posições para o processo 0
        int local_size = local_positions.size();
        MPI_Send(&local_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(local_positions.data(), local_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Liberar memória
    if (rank == 0) {
        delete[] data;
    }
    delete[] local_data;

    MPI_Finalize();
    return 0;
}
