%%writefile exemplo1.cu
#include <thrust/device_vector.h> // Inclui a biblioteca para vetores na GPU (device)
#include <thrust/host_vector.h>   // Inclui a biblioteca para vetores na CPU (host)
#include <iostream>               // Biblioteca padrão para entrada e saída de dados

int main() {
    // Cria um vetor na CPU (host) com 5 elementos, todos inicializados com 0
    thrust::host_vector<double> host(5, 0);
    host[4] = 35; // Altera o último elemento do vetor para 35

    /* Os dados do vetor 'host' são copiados para a GPU,
       criando um vetor equivalente na GPU (device) */
    thrust::device_vector<double> dev(host);

    /* Altera o vetor na CPU, mas não afeta o vetor na GPU,
       pois a cópia já foi feita anteriormente */
    host[2] = 12; // Altera o terceiro elemento do vetor na CPU para 12

    // Exibe os elementos do vetor na CPU
    printf("Host vector: ");
    for (auto i = host.begin(); i != host.end(); i++) {
        std::cout << *i << " "; // Acesso rápido aos elementos na CPU
    }
    printf("\n");

    // Exibe os elementos do vetor na GPU
    printf("Device vector: ");
    for (auto i = dev.begin(); i != dev.end(); i++) {
        std::cout << *i << " "; // Acesso aos elementos na GPU é lento, pois os dados estão na GPU
    }
    printf("\n");
}
