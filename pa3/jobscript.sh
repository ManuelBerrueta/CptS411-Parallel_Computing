#!/bin/sh
#To run this script, please use "sbatch -N<numberofnodes> <path>/sub.sh"

# setting the max wallclock time after which the job will be killed; 
# note: the format is hr:min:sec (default set here to 10 mins)
#SBATCH --time=00:05:00

#"np" stands for number of processes.
#this command will run the job on n processes. 
# $1 is num of (sub)procs, $2 is A, $3 is B, $4 is Prime, $6 is seed, $7 is n
# sample usage: ./jobscript.sh 8 1069 3069 9973 69 8
#   sbatch -N4 jobscript.sh 8 1069 3069 9973 69 8
#   sbatch -N<nodes> jobscript.sh p A B Prime seed n
#   sbatch -N8 jobscript.sh 16 3069 1337 9973 69 16
mpirun -np $1 ~/cpts411/pa3/pa3 $2 $3 $4 $5 $6 >> output.txt

# you can add more lines of commands here, if you wish.

