#!/bin/bash

#SBATCH --nodes=1 --ntasks-per-node=8
#SBATCH --job-name=dgemv

cd $SLURM_SUBMIT_DIR

export OMP_NUM_THREADS=8

./dgemv
