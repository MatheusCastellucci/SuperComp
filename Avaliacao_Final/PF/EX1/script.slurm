#!/bin/bash
#SBATCH --job-name=exercicio1                # Nome do job
#SBATCH --output=output_exercicio1_%j.out    # Nome do arquivo de saída padrão
#SBATCH --nodes=5                            # Número de máquinas/nós
#SBATCH --ntasks-per-node=1                  # Número de cores/tarefas por máquina
#SBATCH --mem=16G                            # Memória por nó
#SBATCH --partition=prova                    # Nome da partição
#SBATCH --time=01:00:00                      # Limite de tempo de execução

export OMP_NUM_THREADS=15

./programa
