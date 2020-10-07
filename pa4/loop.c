/* Example source code generated for Cpt S 411: Intro to Parallel Computing
 * Author: Ananth Kalyanaraman
 * Institution: Washington State University
 * Disclaimer: Use at your own risk!
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>

int main(int argc, char *argv[])
{
	long long int i, loops;

	// loop {number of iterations} [number of threads]

	if(argc<2) {
		printf("Usage: loop {number of iterations} [number of threads]\n");
		exit(1);
	}
	
	loops = atoll(argv[1]);
	printf("Debug: number of iterations = %lld (%d)\n",loops, sizeof(long long int));

	int p=1;
	if(argc==3) {
		p = atoi(argv[2]);
		assert(p>=1);
		printf("Debug: number of requested threads = %d\n",p);
	}

	omp_set_num_threads(p);

	#pragma omp parallel
	{
		assert(p==omp_get_num_threads());
		//printf("Debug: number of threads set = %d\n",omp_get_num_threads());

		int rank = omp_get_thread_num();
		printf("Rank=%d: my world has %d threads\n",rank,p);
	}  // end of my omp parallel region

	printf("Debug: sizeof pointer= %d bytes \n",sizeof(int *));

	int a;

	double time = omp_get_wtime();
	#pragma omp parallel for schedule(static) private(a)	//creates N threads to run the next enclosed block 
	for(i = 0; i < loops; i++)  //or line in parallel
	{	
		a = 6+7*8;
	}
	// end of the second parallel region for FOR LOOP
	
	time = omp_get_wtime() - time;
	printf("\n %f seconds \n ", time);

	return 0;
}
