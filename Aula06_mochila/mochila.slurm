#!/bin/bash
#SBATCH --job-name=mochila            # Nome do job
#SBATCH --output=mochila_%j.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:10:00
#SBATCH --partition=espec


./mochila