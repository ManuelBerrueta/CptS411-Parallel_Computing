#include "matrix.c"

//*Primes list: https://primes.utm.edu/lists/small/10000.txt
//* A=991  B=997 P=9973

unsigned int *serial_baseline(unsigned int n);
unsigned int *serial_matrix(unsigned int n);

extern unsigned int A;
extern unsigned int B;
extern unsigned int Prime;
extern unsigned int seed;

/* int main(int argc, char const *argv[])
{
    // Ignore this
    // int * test = (int *)malloc(sizeof(int) * 2 * 2);
    // ((int(*)[2])test)[1][1] = 7;

    int buf[256];
    unsigned int n;
    int *x_array = { 0 };

    A = atoi(argv[1]);
    B = atoi(argv[2]);
    Prime = atoi(argv[3]);
    seed = atoi(argv[4]);
    n = atoi(argv[5]);

    printf("\nA=%u B=%u P=%u n=%u seed=%u\n", A, B, Prime, n, seed);
    fflush(stdout);

    x_array = serial_baseline(n);

    puts("");
    for(int i = 0; i <= n; i++)
    {
        printf("%d ", x_array[i]);
        fflush(stdout);
    }
    puts("");
    x_array = serial_matrix(n);

    for(int i = 0; i <= n; i++)
    {
        printf("%d ", x_array[i]);
        fflush(stdout);
    }
    puts("");

    return 0;
}
 */
unsigned int * serial_baseline(unsigned int n)
{
    unsigned int *x_array = (unsigned int *) malloc(sizeof(unsigned int) * (n+1));

    x_array[0] = seed;

    for(int i=1; i <= n; i++)
    {
        x_array[i] = ((A * x_array[i-1]) + B) % Prime;
    }
    return x_array;
}

unsigned int * serial_matrix(unsigned int n)
{
    unsigned int * x_array = (unsigned int *) malloc(sizeof(unsigned int) * (n + 1));
    Matrix * M_arr = (Matrix *)malloc(sizeof(Matrix) * (n + 2));
    unsigned int * result = NULL;
    Matrix M;

    //Initialize Matrix
    M.m[0][0] = A;
    M.m[1][0] = B;
    M.m[0][1] = 0;
    M.m[1][1] = 1;

    unsigned int v[2];
    v[0] = seed;
    v[1] = 1;

    M_arr[0] = M;
    M_arr[1] = M;

    x_array[0] = seed;

    for(int i = 1; i <= n; i++)
    {
        x_array[i] = rectModuloMult(v, M_arr[i], Prime);
        M_arr[i + 1] = sqModuloMult(M_arr[i], M, Prime);
    }

    // for (int i = 1; i <= n; i++) {
    //     printf("\t\ti = %d\n", i);
    //     printMatrix(M_arr[i]);
    // }

    return x_array;
}