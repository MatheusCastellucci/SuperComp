#!/bin/bash
#SBATCH --job-name=mochila            # Nome do job
#SBATCH --output=mochila_%j.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec


./mochila