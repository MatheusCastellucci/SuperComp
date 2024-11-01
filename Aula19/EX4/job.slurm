#!/bin/bash
#SBATCH --job-name=ex
#SBATCH --output=output_ex.txt
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=4
#SBATCH --mem=2048
#SBATCH --time=00:05:00
#SBATCH --partition=espec

mpirun -np 2 ./search