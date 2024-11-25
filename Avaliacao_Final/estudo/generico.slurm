#!/bin/bash
#SBATCH --job-name=meu_job       # Nome do job
#SBATCH --output=saida_%j.out   # Nome do arquivo de saída (%j será substituído pelo job ID)
#SBATCH --error=erro_%j.err     # Nome do arquivo de erro
#SBATCH --ntasks=4              # Número de tarefas
#SBATCH --nodes=1               # Número de nós
#SBATCH --time=01:00:00         # Tempo máximo de execução (hh:mm:ss)
#SBATCH --mem=4G                # Memória por nó
#SBATCH --partition=default     # Partição/queue

# Comandos do job
module load gcc                 # Exemplo de carregar um módulo necessário
srun ./meu_programa             # Executa o programa
