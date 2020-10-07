#!/bin/sh

# Script to run the jobscript

rm slurm*
rm output.csv

# sbatch -N<nodes> jobscript.sh p A B Prime seed n

sbatch -N2 jobscript.sh 2 3069 1337 9973 69 1048576
sleep 1
sbatch -N4 jobscript.sh 4 3069 1337 9973 69 1048576
sleep 1
sbatch -N8 jobscript.sh 8 3069 1337 9973 69 1048576
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 1048576
sleep 1
sbatch -N8 jobscript.sh 32 3069 1337 9973 69 1048576
sleep 1
sbatch -N8 jobscript.sh 64 3069 1337 9973 69 1048576
