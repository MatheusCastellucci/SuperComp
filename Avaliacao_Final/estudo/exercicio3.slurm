#!/bin/bash
#SBATCH --job-name=matrix_mul_test       # Nome do job
#SBATCH --output=saida_%j.out            # Nome do arquivo de saída padrão
#SBATCH --error=erro_%j.err              # Nome do arquivo de erro
#SBATCH --partition=prova                # Nome da partição
#SBATCH --nodes=1                        # Usar apenas 1 nó
#SBATCH --cpus-per-task=8                # Máximo de 8 CPUs

# Testar com diferentes números de threads e tamanhos de matriz
for size in 500 1000 1500; do
    for cores in 2 4 8; do
        export OMP_NUM_THREADS=$cores
        echo "Rodando com $cores threads e tamanho da matriz $size x $size"
        ./matrix $size
    done
done
