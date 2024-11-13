#!/bin/bash
#SBATCH --job-name=abordagemExaustiva1           # Nome do job
#SBATCH --output=output_abordagemExaustiva1.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=2048
#SBATCH --time=02:00:00
#SBATCH --partition=espec


./abordagem_exaustiva1

./abordagem_exaustiva2