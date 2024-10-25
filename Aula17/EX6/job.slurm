#!/bin/bash
#SBATCH --job-name=ex
#SBATCH --output=output_ex.txt
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024
#SBATCH --time=00:05:00
#SBATCH --partition=espec

mpirun ./ex