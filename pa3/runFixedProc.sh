#!/bin/sh

# Script to run the jobscript

rm slurm*
rm output.csv

# sbatch -N<nodes> jobscript.sh p A B Prime seed n

sbatch -N8 jobscript.sh 16 3069 1337 9973 69 16
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 32
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 64
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 128
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 256
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 512
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 1024
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 2048
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 4096
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 8192
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 16384
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 32768
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 65536
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 131072
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 262144
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 524288
sleep 1
sbatch -N8 jobscript.sh 16 3069 1337 9973 69 1048576
