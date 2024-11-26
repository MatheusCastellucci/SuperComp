#!/bin/bash
#SBATCH --job-name=exericio2
#SBATCH --output=output_exercicio2.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=2048
#SBATCH --time=01:00:00
#SBATCH --partition=espec


./parte_a
./parte_b
./parte_c
./parte_d