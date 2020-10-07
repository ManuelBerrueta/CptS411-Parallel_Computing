#!/bin/sh

# Script to run the jobscript

rm slurm*
#rm output.csv

sbatch -N$1 jobscript.sh $2 1024
sleep 1
sbatch -N$1 jobscript.sh $2 2048
sleep 1
sbatch -N$1 jobscript.sh $2 4096
sleep 1
sbatch -N$1 jobscript.sh $2 8192
sleep 1
sbatch -N$1 jobscript.sh $2 16384
sleep 1
sbatch -N$1 jobscript.sh $2 32768
sleep 1
sbatch -N$1 jobscript.sh $2 65536
sleep 1
sbatch -N$1 jobscript.sh $2 131072
sleep 1
sbatch -N$1 jobscript.sh $2 262144
sleep 1
sbatch -N$1 jobscript.sh $2 524288
sleep 1
sbatch -N$1 jobscript.sh $2 1048576
