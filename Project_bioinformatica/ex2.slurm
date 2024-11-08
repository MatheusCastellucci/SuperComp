#!/bin/bash
#SBATCH --job-name=transcricao           # Nome do job
#SBATCH --output=output_transcriao.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=2048
#SBATCH --time=02:00:00
#SBATCH --partition=espec

mpirun -np 4 ./exercicio2