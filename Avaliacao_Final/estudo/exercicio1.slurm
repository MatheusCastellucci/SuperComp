#!/bin/bash
#SBATCH --job-name=exercicio1                # Nome do job
#SBATCH --output=output_exercicio1_%j.out    # Nome do arquivo de saída padrão
#SBATCH --nodes=2                            # Número de máquinas/nós
#SBATCH --ntasks-per-node=5                  # Número de cores/tarefas por máquina
#SBATCH --mem=3G                             # Memória por nó (3GB)
#SBATCH --partition=prova                    # Nome da partição

# Exportar variável de ambiente OMP_NUM_THREADS
export OMP_NUM_THREADS=10

# Comando para executar o código fictício
./executavel
