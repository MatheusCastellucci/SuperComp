#!/bin/bash
#SBATCH --job-name=project_mpi
#SBATCH --output=output_mpi.txt
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024
#SBATCH --time=00:05:00
#SBATCH --partition=espec

mpirun -np 2 ./openmp_mpi