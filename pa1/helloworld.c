/*  Cpt S 411, Introduction to Parallel Computing
 *  School of Electrical Engineering and Computer Science
 *    
 *  Example code
 *  	Hello world MPI
 *          
 */


#include <stdio.h>
#include <mpi.h> 

int main(int argc,char *argv[])
{

   int rank,p;

   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&p);

   printf("my rank=%d\n",rank);
   printf("Rank=%d: number of processes =%d\n",rank,p);

   //while(1) {
   //}

   MPI_Finalize();
}
