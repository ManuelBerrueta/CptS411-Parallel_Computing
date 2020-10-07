#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>
#include <math.h>

#include "distanceFormula.c"

//To compile an OpenMP code use:
                //gcc -o {execname} -fopenmp {source file names}

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Need command line arguments n=#_of_darts and p=#_of_threads\n");
        return -1;
    }

    int n = 0, p = 0;
    p = atoi(argv[1]);
    n = atoi(argv[2]);

    omp_set_num_threads(p);

	#pragma omp parallel
	{
		assert(p == omp_get_num_threads());
		//printf("Debug: number of threads set = %d\n",omp_get_num_threads());

		//int rank = omp_get_thread_num();
		//printf("Rank=%d: my world has %d threads\n", rank, p);
	}  // end of my omp parallel region

    int i = 0, sum = 0;

    double startTime = omp_get_wtime();
    int timeSince1970 = time(NULL);

    #pragma omp parallel shared(n) private(i)
    {
        double x = 0.0, y = 0.0;
        int rank = omp_get_thread_num();
        int seed = rank + timeSince1970;

        //#pragma omp parallel for shared(n) private(i, x, y) reduction(+:sum)
        #pragma omp for private(x, y) reduction(+:sum)
        for (i = 0; i < n; i++) {
            x = (double)rand_r(&seed) / RAND_MAX;
            y = (double)rand_r(&seed) / RAND_MAX;
            //printf("  x = %lf, y = %lf\n", x, y);

            if (getDistance(x, y) <= 0.5) {
                sum++;
            }
        } // end #pragma omp for
    } // end #pragma omp parallel

    double duration = omp_get_wtime() - startTime;

    printf("total successes = %d\n", sum);
    double piEst = (double)sum * 4 / n;
    printf("estimated value of pi = %0.12lf\n", piEst);

    double piTruth = 3.141592653589793;
    // printf("piTruth = %0.15lf\n", piTruth);
    // printf("piTruth = 3.141592653589793\n");

    char bufEst[18] = "", bufTruth[18] = "";
    sprintf(bufEst, "%0.15lf", piEst);
    sprintf(bufTruth, "%0.15lf", piTruth);

    for (i = 0; i < 15; i++) {
        if ((bufEst + 2)[i] != (bufTruth +2)[i]) {
            break;
        }
    }
    printf("piEst is accurate to %d decimal places\n", i);

    printf("time to execute threaded code: %0.3lf s \n", duration);

    return 0;
}
