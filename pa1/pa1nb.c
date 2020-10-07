/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *	
 *	Example code
 *	Send receive test:
 *   	rank 1 sends to rank 0 (all other ranks sit idle)
 *   	For timing use of C gettimeofday() is recommended.
 * */

#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{

	int rank, p;
	struct timeval t1, t2;

	FILE * sendOutput = fopen("OutputSendNonBlocking.csv", "w");
	FILE * recvOutput = fopen("OutputRecvNonBlocking.csv", "w");

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	int i = 0;
	for ( ; i < 10; i++) {

		printf("my rank=%d\n", rank);
		printf("Rank=%d: number of processes =%d\n", rank, p);

		assert(p >= 2);

		if (rank == 1)
		{
			int target = 1; // num bytes to send for the current iteration
			int bytesToSend = 0; // bytes to send in each loop during the current iteration
			int bytesSent = 0; // bytes sent so far for each iteration
			int maxSend = 2000000; // representing 2 MB
			char sendBuf[1024] = { 0 };
			int tSend, dest = 0;

			do { // for all iterations 1, 2, 4, 8, ... up to 2 MB
				bytesSent = 0;

				gettimeofday(&t1, NULL);

				while (bytesSent < target) {
					if (target - bytesSent >= 1024) {
						bytesToSend = 1024;
					}
					else { // should not need to execute when using powers of 2
						bytesToSend = target - bytesSent;
					}

					// MPI_Isend() gave error of too few args though docs said same num of args
					MPI_Send(sendBuf, bytesToSend, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
					bytesSent += bytesToSend;
				}

				gettimeofday(&t2, NULL);
				
				//tSend = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
				tSend = (t2.tv_usec - t1.tv_usec);
				printf("%d bytes sent in %d microseconds\n", bytesSent, tSend);
				fprintf(sendOutput, "%d,%d\n", tSend, bytesSent);

				target *= 2;
			} while (target < maxSend);

			// int tSend = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;

			// printf("Rank=%d: sent message %d to rank %d; Send time %d millisec\n", rank, x, dest, tSend);
		}
		else if (rank == 0)
		{
			
			// MPI_Status status; // for MPI_Recv
			MPI_Request recv_request;

			int target = 1; // num bytes to send for the current iteration
			int bytesToRecv = 0; // bytes to send in each loop during the current iteration
			int bytesRecvd = 0; // bytes sent so far for each iteration
			int maxRecv = 2000000; // representing 2 MB
			char recvBuf[1024] = { 0 }, readTest = '\0';
			int tRecv, dest = 0;

			do { // for all iterations 1, 2, 4, 8, ... up to 2 MB
				bytesRecvd = 0;

				gettimeofday(&t1, NULL);

				while (bytesRecvd < target) {
					if (target - bytesRecvd >= 1024) {
						bytesToRecv = 1024;
					}
					else { // should not need to execute when using powers of 2
						bytesToRecv = target - bytesRecvd;
					}

					MPI_Irecv(recvBuf, bytesToRecv, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_request);
					readTest = recvBuf[bytesToRecv - 1];
					bytesRecvd += bytesToRecv;
				}

				gettimeofday(&t2, NULL);
				
				//tRecv = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
				tRecv = (t2.tv_usec - t1.tv_usec);
				printf("%d bytes received in %d microseconds\n", bytesRecvd, tRecv);
				fprintf(recvOutput, "%d,%d\n", tRecv, bytesRecvd);

				target *= 2;
			} while (target < maxRecv);

			// int tRecv = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
			// printf("Rank=%d: received message %d from rank %d; Recv time %d millisec\n", rank, y, status.MPI_SOURCE, tRecv);
		}

		fprintf(sendOutput, "\n");
		fprintf(recvOutput, "\n");
	}

	MPI_Finalize();

	fclose(sendOutput);
	fclose(recvOutput);
}
