#!/bin/bash
#SBATCH --job-name=mpi_openmp_job       # Nome do job
#SBATCH --output=saida_%j.out          # Nome do arquivo de saída padrão
#SBATCH --ntasks=4                     # Número total de processos MPI
#SBATCH --cpus-per-task=5              # Número de threads por tarefa
#SBATCH --time=00:30:00                # Tempo máximo de execução
#SBATCH --mem=2G                       # Memória por nó
#SBATCH --partition=prova              # Partição usada

# Exportar a variável para o número de threads OpenMP
export OMP_NUM_THREADS=5

# Executar o programa MPI
mpirun ./exercicio2
