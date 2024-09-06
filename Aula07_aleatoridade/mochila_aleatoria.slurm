#!/bin/bash
#SBATCH --job-name=mochila_aleatoria            # Nome do job
#SBATCH --output=mochila_aleatoria_%j.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=03:00:00
#SBATCH --partition=espec


./mochila_aleatoria2