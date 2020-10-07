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

int p=1;

int *generateArray(int);
void dispArray(int *,int);
void computeSum(int *,int );

int main(int argc, char *argv[])
{
	int i;
	int  n;

	// loop {number of iterations} [number of threads]

	if(argc<2) {
		printf("Usage: sumcomp {array size} [number of threads]\n");
		exit(1);
	}
	
	n = atoll(argv[1]);
	printf("Debug: array size = %d \n",n);

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

	double time = omp_get_wtime();

	// 1. generate the array
	//
	//
	int *a=NULL;
	a = generateArray(n);

	//dispArray(a,n);

	// 2. compute sum using reduce
	computeSum(a,n);


	
	time = omp_get_wtime() - time;
	printf("Total time = %f seconds \n ", time);

	return 0;
}

int *generateArray(int n) {

	int *a = (int *)malloc(sizeof(int)*n);
	assert(a!=NULL);

	printf("n=%d, p=%d\n",n,p);

	int i=0;

	omp_set_num_threads(p);

	#pragma omp parallel default(none) shared(a,n) private (i)
	{

		#pragma omp for 
		for(i = 0; i < n; i++)  //or line in parallel
		{

			int rank = omp_get_thread_num();
			int seed = rank+1;
			seed = seed*i;
			
			//printf("rank=%d: seed = %d, i=%d,n=%d\n",rank,seed,i,n);
			a[i] = rand_r(&seed)%20;
		}

	}

	return a;

	
} // end generateArray

void computeSum(int *a,int n) {

	omp_set_num_threads(p);

	int i;
	int sum=0;


	#pragma omp parallel for shared(a,n) reduction(+:sum) 
	for(i=0;i<n;i++) {
		//printf("i=%d, n=%d, sum=%d,a[i]=%d\n",i,n,sum,a[i]);
		//fflush(stdout);
		sum+=a[i];
	}

		
	printf("sum=%d\n",sum);


}// end computeSum

void dispArray(int *a,int n) {
	int i;
	printf("********************\n");
	printf("Debug: Array a is: \n");
	for(i=0;i<n;i++) {
		printf("%d\n",a[i]);
	}
	printf("********************\n");
}

