#include <stdio.h>
#include <stdlib.h>
#include <math.h> //! with gcc must link with -lm as last flag
#include <time.h>
#include <mpi.h>

double Log2( double n );
int *GenerateArray(int arraySize, int rank);
int NaiveReduce(int listSize, int rank, int p);
int MyReduce(int * arr, int n, int rank, int p);
int MPILibraryReduce(int listSize);

int main(int argc, char const *argv[])
{
    int i = 0;
    int n = 64;
    int total = 0;
    int* numsList = 0;
    int rank = -1;
    int p = 0;

    //! MPI Commands
    //MPI_Status(status);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc > 1)
    {
        n = atoi(argv[1]);
        //NaiveReduce(n);
    }
    else
    {
        //numsList = GenerateArray(n, 0);
        NaiveReduce(n, rank, p);
    }

    MPI_Finalize();
    return 0;
}


//REF: https://stackoverflow.com/questions/758001/log2-not-found-in-my-math-h
double Log2( double n )  
{  
    return log( n ) / log( 2.0 );  
}  

int *GenerateArray(int arraySize, int rank)
{
    /*! Side notes: The malloc() function allocates memory and leaves the memory
     uninitialized. Whereas, the calloc() function allocates memory and 
     initializes all bits to zero. */
    
    //int *numList = (int *) malloc(arraySize * sizeof(int)); //Using malloc
    int *numList = (int *) calloc(arraySize / 8,sizeof(int)); //Using calloc
    if(!numList)
    {
        puts("Dynamic Memory allocation failed!!\n\n");
    }
    int i = 0;
    
    //! Populate the array with pseudo random numbers using C PRNG
    //? NOTE: Not safe for crypto!
    srand(time(NULL)*(rank+1)); //* Initialize / Could also use the rank
    //srand(time(rank+1));
    while( i < arraySize / 8)
    {
        numList[i] = rand() % 10000;
        printf("Rank = %d, List[%d] = %d\n", rank, i, numList[i]);
        i++;
    }



    return numList;
}

//! Trivial Parallel Algorithm Approach
//* Input n integers, output sum of n integers
int NaiveReduce(int n, int rank, int p)
{
    int i=0;
    int sum = 0;
    int incomingSum = 0;
    int destination = -1;
    int *numsList = 0;

    
    int bytesToRecv = 4; // bytes to send in each loop during the current iteration
    int bytesRecvd = 0; // bytes sent so far for each iteration
    //TODO: Possible cast as (char *) to send 1 byte at a time?
    int recvBuff = -1; // Allows us to send 1 byte at a time
    char sendBuff[1024] = { 0 }; // Allows us to send 1 byte at a time
    int bytesToSend = 4;


    printf("before gen, rank= %d", rank);
    numsList = GenerateArray(n, rank);

    //!================= Testing Zone ===========================
    
    i=0;
    while(i < n/8)
    {
        sum += numsList[i];
        i++;
    }
    printf("TOTAL = %d\n", sum);

    //!================= End Testing Zone ===========================

    //TODO: Do a calculation sum for this proces

    if (rank > 0)
    {
        
        //! MPI Command
        MPI_Status status;

        int sourceProcess = rank-1;
        MPI_Recv(&recvBuff, bytesToRecv, MPI_INT, sourceProcess, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        sum += recvBuff;

        if (rank < (p - 1)) //! If rank is less than the # of procs, send my sum to next proc
        {
            destination = rank+1;
            MPI_Send(&sum, bytesToSend, MPI_INT, destination, 0, MPI_COMM_WORLD);
        }
    }
    else if ( rank == 0)
    {
        destination = rank+1;
        MPI_Send(&sum, bytesToSend, MPI_INT, destination, 0, MPI_COMM_WORLD);
    }
    if (rank == (p - 1))
    {
        printf("The sum of the array using the Naive Approach is: %d\n", sum);
    }
    return 1;
}


int MyReduce(int * arr, int n, int rank, int p)
{
    int i = 0, partner = -1;
    int sum = 0, numNodes = 0;
    int destination = -1;

    int bytesToRecv = 4; // bytes to send in each loop during the current iteration
    //TODO: Possible cast as (char *) to send 1 byte at a time?
    int recvBuff = -1; // Allows us to send 1 byte at a time
    int bytesToSend = 4;
    MPI_Status status;

    if (p >= 8) {
        numNodes = 8;
    }
    else {
        numNodes = p;
    }

    for (int i = 0; i < n / numNodes; i++)
    {
        sum += arr[i];
    }
    printf("    Rank %d Subtotal = %d\n", sum);


    for (int t = 1; t <= Log2(p); t <<= 1) // For each time step
    {
        partner = rank ^ t;

        if (partner < rank) { // receive from partner
            //incomingSum = MPI_SendRecv(recvBuff, bytesToRecv, MPI_INT, partner, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(&recvBuff, bytesToRecv, MPI_INT, partner, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            sum += recvBuff;
        }
        else { // send to partner
            //MPI_SendRecv(sum, bytesToSend, MPI_INT, partner, 0, MPI_COMM_WORLD);
            MPI_Send(&sum, bytesToSend, MPI_INT, destination, 0, MPI_COMM_WORLD);
        }
    }
    printf("Rank %d sum = %d\n", rank, sum);
}

int MPILibraryReduce(int listSize)
{
    int i, rank, partner, n, p, ierr;
    //?MPI_Status status;
    //?MPI_Init(&argc, &argv);
    //?MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //?MPI_Comm_size(MPI_COMM_WORLD, &p);
    int *numsList = GenerateArray(listSize, rank);

    //TODO: Must slice the array?


    //MPI_Allreduce(sum, c, n, MPI_REAL, MPI_SUM, MPI_COMM_WORLD, ierr)
}


/*  !NOTES  https://www.mpi-forum.org/docs/mpi-2.2/mpi22-report/node109.htm
            https://www.mpich.org/static/docs/v3.2.x/www3/MPI_Allreduce.html
            http://mpi.deino.net/mpi_functions/MPI_Allreduce.html
 */