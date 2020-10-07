#include <stdio.h>
#include <stdlib.h>
#include <math.h> //! with gcc must link with -lm as last flag
#include <time.h>
#include <mpi.h>
#include <sys/time.h>

double Log2( double n );
int * GenerateArray(int n, int rank, int numNodes);
int NaiveReduce(int listSize, int rank, int p, int * arr, int numNodes, long (*op)(long, long));
int MyReduce(int * arr, int n, int rank, int p, int numNodes, long (*op)(long, long));
int MPILibraryReduce(int * arr, int n, int numNodes, long (*op)(long, long));
int driver(int argc, char const *argv[], long (*op)(long, long));

long add(long n1, long n2) {
    return n1 + n2;
}

long max(long n1, long n2) {
    return n1 >= n2 ? n1 : n2;
}

int main(int argc, char const *argv[]) {
    return driver(argc, argv, add);
}

int driver(int argc, char const *argv[], long (*op)(long, long))
{
    int i = 0;
    int n = 1048576, numNodes = 0;
    int total = 0;
    int* numsList = 0;
    int rank = -1;
    int p = 0;
    struct timeval t1, t2;
    int tSend;


    //! MPI Commands
    //MPI_Status(status);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (p >= 8) {
        numNodes = 8;
    }
    else {
        numNodes = p;
    }

    if (argc > 1)
    {
        n = atoi(argv[1]);
    }
    //printf("argc = %d, argv[1] = %s, n = %d\n", argc, argv[1], n);
    numsList = GenerateArray(n, rank, numNodes);

    // i=0;
    // printf("Number List = { ");
    // while(i < n/8)
    // {
    //     printf("%d", numsList[i]);
    //     i++;
    //     if(i != n/8)
    //     {
    //         printf(", ");
    //     }
    // }
    // printf(" }\n");
    printf("method,p,n,rank,microsec\n");
    
    gettimeofday(&t1, NULL);//TODO: TIME START
    NaiveReduce(n, rank, p, numsList, numNodes, op);
    gettimeofday(&t2, NULL);//TODO: TIME END

    tSend = (t2.tv_usec - t1.tv_usec);
    printf("NaiveReduce,%d,%d,%d,%d\n", p, n, rank, tSend);
    //printf("        NaiveReduce ran in %d microseconds\n", tSend);


    gettimeofday(&t1, NULL);//TODO: TIME START
    MyReduce(numsList, n, rank, p, numNodes, op);
    gettimeofday(&t2, NULL);//TODO: TIME END

    tSend = (t2.tv_usec - t1.tv_usec);
    printf("MyReduce,%d,%d,%d,%d\n", p, n, rank, tSend);
    //printf("        MyReduce ran in %d microseconds\n", tSend);


    gettimeofday(&t1, NULL);//TODO: TIME START
    MPILibraryReduce(numsList, n, numNodes, op);
    gettimeofday(&t2, NULL);//TODO: TIME END

    tSend = (t2.tv_usec - t1.tv_usec);
    printf("MPI_Allreduce,%d,%d,%d,%d\n\n", p, n, rank, tSend);
    //printf("        MPILibraryReduce ran in %d microseconds\n", tSend);

    MPI_Finalize();
    return 0;
}

//REF: https://stackoverflow.com/questions/758001/log2-not-found-in-my-math-h
double Log2( double n )  
{  
    double dividend = log(n), divisor = log(2.0);
    double quotient = dividend / divisor;

    return quotient;  
}  

int * GenerateArray(int n, int rank, int numNodes) {
    /*! Side notes: The malloc() function allocates memory and leaves the memory
     uninitialized. Whereas, the calloc() function allocates memory and 
     initializes all bits to zero. */
    
    int * numList = (int *) calloc(n / numNodes,sizeof(int)); //Using calloc
    if(!numList)
    {
        puts("Dynamic Memory allocation failed!!\n\n");
    }
    int i = 0;
    
    //! Populate the array with pseudo random numbers using C PRNG
    //? NOTE: Not safe for crypto!
    srand(time(NULL)*(rank+1)); //* Initialize / Could also use the rank
    //srand(time(rank+1));
    while( i < n / numNodes)
    {
        numList[i] = rand() % 10000;
        //printf("Rank = %d, List[%d] = %d\n", rank, i, numList[i]);
        i++;
    }
    return numList;
}

//! Trivial Parallel Algorithm Approach
//* Input n integers, output sum of n integers
int NaiveReduce(int n, int rank, int p, int * arr, int numNodes, long (*op)(long, long)) {
    int i=0;
    long sum = 0;
    int incomingSum = 0;
    int destination = -1;

    //TODO: Possible cast as (char *) to send 1 byte at a time?
    long recvBuff = -1; // Allows us to send 1 byte at a time
    char sendBuff[1024] = { 0 }; // Allows us to send 1 byte at a time

    for (int i = 0; i < n / numNodes; i++)
    {
        sum = op(sum, arr[i]);
    }
    //printf("    Naive rank %d subtotal = %d\n", rank, sum);

    if (rank > 0)
    {       
        //! MPI Command
        MPI_Status status;

        int sourceProcess = rank-1;
        MPI_Recv(&recvBuff, 1, MPI_LONG, sourceProcess, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        sum = op(sum, recvBuff);

        if (rank < (p - 1)) //! If rank is less than the # of procs, send my sum to next proc
        {
            destination = rank+1;
            MPI_Send(&sum, 1, MPI_LONG, destination, 0, MPI_COMM_WORLD);
        }
    }
    else if ( rank == 0)
    {
        destination = rank+1;
        MPI_Send(&sum, 1, MPI_LONG, destination, 0, MPI_COMM_WORLD);
    }
    if (rank == (p - 1))
    {
        printf("Naive rank %d final sum = %d\n", rank, sum);
    }
    return 1;
}

int MyReduce(int * arr, int n, int rank, int p, int numNodes, long (*op)(long, long)) {
    int i = 0;
    unsigned short partner = 0;
    long sum = 0, recvSum = 0;
    int destination = -1;

    MPI_Status status;

    for (int i = 0; i < n / numNodes; i++)
    {
        sum = op(sum, arr[i]);
    }
    //printf("    MyReduce rank %d subtotal = %d\n", rank, sum);

    for (unsigned short t = 1; t <= numNodes / 2; t <<= 1) // For each time step
    {
        partner = (unsigned short)rank ^ t;

        //printf("  rank %d, t = %d, partner = %d\n", rank, t, partner);

        MPI_Sendrecv(&sum, 1, MPI_LONG, partner, 0, 
            &recvSum, 1, MPI_LONG, partner, 0, 
            MPI_COMM_WORLD, &status);
        sum = op(sum, recvSum);
    }
    printf("MyReduce rank %d final sum = %d\n", rank, sum);
}

int MPILibraryReduce(int * arr, int n, int numNodes, long (*op)(long, long))
{
    long sum = 0, recvSum = -1; // Allows us to send 1 byte at a time
    char sendBuff[1024] = { 0 }; // Allows us to send 1 byte at a time

    for (int i = 0; i < n / numNodes; i++)
    {
        sum = op(sum, arr[i]);
    }

    if (op == &add) {
        MPI_Allreduce(&sum, &recvSum, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
    }
    else if (op == &max) {
        MPI_Allreduce(&sum, &recvSum, 1, MPI_LONG, MPI_MAX, MPI_COMM_WORLD);
    }

    printf("MPILibraryReduce sum: %d\n", recvSum);
}


/*  !NOTES  https://www.mpi-forum.org/docs/mpi-2.2/mpi22-report/node109.htm
            https://www.mpich.org/static/docs/v3.2.x/www3/MPI_Allreduce.html
            http://mpi.deino.net/mpi_functions/MPI_Allreduce.html
 */