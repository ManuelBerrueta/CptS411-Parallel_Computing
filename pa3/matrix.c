#ifndef HEADERS
#define HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <math.h> //! with gcc must link with -lm as last flag
#include <time.h>
#include <sys/time.h>

#endif

typedef struct matrix {
    unsigned int m[2][2];
} Matrix;

// int ** sqMatrixAssign(int M1[][2], int M2[][2]) {
//     M1[0][0] = M2[0][0];
//     M1[0][1] = M2[0][1];
//     M1[1][0] = M2[1][0];
//     M1[1][1] = M2[1][1];

//     return M1;
// }

// Returns a 1 x 2 int array
unsigned int * rectMatrixMult(unsigned int M1[], Matrix M2) {
    unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * 2);

    R[0] = M1[0] * M2.m[0][0] + M1[1] * M2.m[1][0];
    R[1] = M1[0] * M2.m[0][1] + M1[1] * M2.m[1][1];

    return R;
}

// Returns a 2 x 2 Matrix
Matrix sqMatrixMult(Matrix M1, Matrix M2) {
    Matrix R;

    R.m[0][0] = M1.m[0][0] * M2.m[0][0] + M1.m[0][1] * M2.m[1][0];
    R.m[0][1] = M1.m[0][0] * M2.m[0][1] + M1.m[0][1] * M2.m[1][1];
    R.m[1][0] = M1.m[1][0] * M2.m[0][0] + M1.m[1][1] * M2.m[1][0];
    R.m[1][1] = M1.m[1][0] * M2.m[0][1] + M1.m[1][1] * M2.m[1][1];

    return R;
}

// Performs modulo multiplication arithmetic for 
//   1 x 2 matrix modulo mult by 2 x 2 matrix
unsigned int rectModuloMult(unsigned int * M1, Matrix M2, unsigned int Prime) {
    unsigned int * R = rectMatrixMult(M1, M2);

    unsigned int result = R[0] % Prime;
    free(R);

    return result;
}

// Performs modulo multiplication arithmetic for 
//   2 x 2 matrix modulo mult by 2 x 2 matrix
Matrix sqModuloMult(Matrix M1, Matrix M2, unsigned int Prime) {
    Matrix R = sqMatrixMult(M1, M2);

    R.m[0][0] %= Prime;
    R.m[0][1] %= Prime;
    R.m[1][0] %= Prime;
    R.m[1][1] %= Prime;

    return R;
}

//void printMatrix(int M[][2]) {
void printMatrix(Matrix M, FILE * outfile) {
    fprintf(outfile, 
        "  | %4d  %4d |\n  | %4d  %4d |\n", M.m[0][0], M.m[0][1], M.m[1][0], M.m[1][1]);
}

/* int main(void) {
    int M1[2][2] = { {1, 2}, {3, 4} };
    int M2[2][2] = { {3, 2}, {5, 4} };

    //printMatrix((int (*)[2])matrixMult(M1, M2));
    printMatrix(sqMatrixMult(M1, M2));

    return 0;
} */