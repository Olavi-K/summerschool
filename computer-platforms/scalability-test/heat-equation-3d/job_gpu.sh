#!/bin/bash
#SBATCH --job-name=example
#SBATCH --account=project_465001194
#SBATCH --error=%x.%J.err
#SBATCH --output=%x.%J.out
#SBATCH --partition=dev-g
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --gpus-per-node=8
##SBATCH --reservation=CSC_summer_school_gpu

srun heat_hip
