// CptS 411 Proj 3
// Manny Berrueta & Ryan Neisess

#include "serial.c"
#include <mpi.h>
#include <string.h>

double Log2( double n );
int ParallelPrefix(int rank);
int driver(int argc, char const *argv[], long (*op)(long, long));

//! Globals
unsigned int A = 0;
unsigned int B = 0;
unsigned int Prime = 0;
unsigned int seed = 0;
unsigned int n = 0;
unsigned int p = 0;
unsigned int seedVect[2] = { 0 };

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
    int rank = -1;
    struct timeval t1, t2;
    int tSend;

    // Ignore this
    // int * test = (int *)malloc(sizeof(int) * 2 * 2);
    // ((int(*)[2])test)[1][1] = 7;

    //int buf[256];

    if (argc == 6) {
        A = atoi(argv[1]);
        B = atoi(argv[2]);
        Prime = atoi(argv[3]);
        seed = atoi(argv[4]);
        n = atoi(argv[5]);

        seedVect[0] = seed;
        seedVect[1] = 1;

        // printf("\nA=%u B=%u P=%u n=%u seed=%u\n", A, B, Prime, n, seed);
        // fflush(stdout);
    }
    else {
        printf("  Improper usage; current argc = %d\n", argc);
        for (int i = 0; i < argc; i++) {
            printf("    %s\n", argv[i]);
        }
        return -1;
    }

    //! MPI Commands
    //MPI_Status(status);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

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
    
    //printf("method,subProcs,n,rank,microsec\n");

/*     gettimeofday(&t1, NULL);// TIME START
    ParallelPrefix(rank);
    gettimeofday(&t2, NULL);// TIME END

    tSend = (t2.tv_usec - t1.tv_usec);
    //printf("ParallelPrefix,%d,%d,%d,%d\n", subProcs, n, rank, tSend);
    //printf("        MyReduce ran in %d microseconds\n", tSend);

    printf("%d,%d,%d,%d\n", p, n, rank, tSend);
 */
    if (rank == 0) {
        unsigned int * checkArr;
        FILE * baseline = fopen("baseline.txt", "w");
        FILE * matrix = fopen("matrix.txt", "w");

        //printf("  Serial baseline: \n");
        gettimeofday(&t1, NULL);// TIME START
        checkArr = serial_baseline(n);
        gettimeofday(&t2, NULL);// TIME END

        tSend = (t2.tv_usec - t1.tv_usec);
        printf("Serial baseline time (us) for %d elements = %d\n", n, tSend);

        for(int i = 0; i <= n; i++)
        {
            //fprintf(baseline, "  %4d  x%d\n", checkArr[i], i);
            fprintf(baseline, "%d\n", checkArr[i]);
        }
        
        //printf("  Serial matrix: \n");
        checkArr = serial_matrix(n);

        for(int i = 0; i <= n; i++)
        {
            //fprintf(matrix, "  %4d  x%d\n", checkArr[i], i);
            fprintf(matrix, "%d\n", checkArr[i]);
        }
        //printf("\n");

        fclose(baseline);
        fclose(matrix);
    }

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

char * _itoa(unsigned int num, char * str, unsigned int base) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    int diff = 0, i = 0;
    char temp[24] = "";

    while (num != 0) {
        diff = num % base;
        temp[i++] = '0' + diff;
        num /= base;
    }
    temp[i--] = '\0';

    int len = strlen(temp), j = 0;
    for (; j < len; j++, i--) {
        str[j] = temp[i];
    }
    str[j] = '\0';
    return str;
}

int ParallelPrefix(int rank) {
    FILE * outfile = NULL;

    if (p == 16 && n == 64) {
        char filename[18] = "Results";
        char rankStr[6] = "";
        _itoa(rank, rankStr, 10);
        //rankStr[0] = '0' + rank;
        //rankStr[1] = '\0';
        strcat(filename, rankStr);
        strcat(filename, ".txt");
        outfile = fopen(filename, "w");
    }

    unsigned short partner = 0;
    long sum = 0, recvSum = 0;
    int destination = -1;
    unsigned int * randArr = (unsigned int *)malloc((n / p) * sizeof(unsigned int));

    Matrix * M_arr = (Matrix *)malloc((n / p) * sizeof(Matrix));
    Matrix M = { A, 0, B, 1 }, M0 = { 1, 0, 0, 1 };
    Matrix M_local = M0, M_global = { 0 };

    M_arr[0] = M;
    for (int i = 1; i < n / p; i++) {
        M_arr[i] = sqModuloMult(M_arr[i - 1], M, Prime);
    }
    // Now each proc mas M^1, M^2, ... M^(n/p) in M_arr

    M_global = M_arr[n / p - 1];
    // Now M_local = M0 and M_global = M^(n/p)

    MPI_Status status;

    Matrix M_send = { 0 }, M_recv = { 0 };
    unsigned short t = 1;
    for (int i = 0; i < Log2(p); i++) // For each time step
    {
        partner = (unsigned short)rank ^ t;
        M_send = M_global;

        //printMatrix(M_send, outfile);

        //printf("  rank %d, t = %d, partner = %d\n", rank, t, partner);

        /* Recall: 
        int MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
            int dest, int sendtag,
            void *recvbuf, int recvcount, MPI_Datatype recvtype,
            int source, int recvtag,
            MPI_Comm comm, MPI_Status *status)
        */
        MPI_Sendrecv(M_send.m, 4, MPI_INT, partner, 0, 
            M_recv.m, 4, MPI_INT, partner, 0, 
            MPI_COMM_WORLD, &status);

        //printMatrix(M_recv, outfile);
        
        M_global = sqModuloMult(M_global, M_recv, Prime);
        if (rank > partner) {
            M_local = sqModuloMult(M_local, M_recv, Prime);
        }

        t <<= 1;
    }
    // Now offsets have been determined

    for (int i = 0; i < n / p; i++) {
        M_arr[i] = sqModuloMult(M_arr[i], M_local, Prime);
        randArr[i] = rectModuloMult(seedVect, M_arr[i], Prime);
    }

    if (p == 16 && n == 64) {
        //fprintf(outfile, "rank %d rand nums: \n", rank);
        for (int i = 0; i < n / p; i++) {
            //fprintf(outfile, "  %4d x%d\n", randArr[i], (rank * n / p) + i) + 1);
            fprintf(outfile, "%d\n", randArr[i]);
        }
        //printf("\n");
        fclose(outfile);
    }

    return 1;
}

/*  !NOTES  https://www.mpi-forum.org/docs/mpi-2.2/mpi22-report/node109.htm
            https://www.mpich.org/static/docs/v3.2.x/www3/MPI_Allreduce.html
            http://mpi.deino.net/mpi_functions/MPI_Allreduce.html
 */