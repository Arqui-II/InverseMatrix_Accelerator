#include "sys/alt_stdio.h"
#include "altera_avalon_performance_counter.h"
#include "system.h"
#include <stdio.h>

//Section of performance counter
#define IDENTITY_BUILD_SECTION 1
#define STORE_MATRIX_SECTION 2
#define INVERSE_SECTION 3

#define N 8
#define MATRIX_OFFSET 0
#define IDENTITY_OFFSET 0x1000

volatile float *ptr = (float*) RAM_BASE;

//float inputMatrix[N*N] = {7, -2, 								// 2x2
//						  3,  5};

//float inputMatrix[N*N] = {3,  2, -5, 							// 3x3
//						  1, -3,  2,
//						  5, -1,  4};

//float inputMatrix[N*N] = {1.4,  6.4,  9.1, 0,					// 4x4
//						  3.2,  2.1,  3.6, 0,
//						 -1.8,  0.3,  0.1, 0,
//						  5.4, -0.2, -0.5, 1};

//float inputMatrix[N*N] = {-3,  3,   6,  9,   12,  15,			// 6x6
//						  -1, -3,   6, 11,   16,  21,
//						   1, -3,  -5,  6,   12,  18,
//						   3, -3,  -9, -12,   0,   6,
//						   5, -3, -13, -22, -27, -15,
//						   7, -3, -17, -32, -45, -53};

float inputMatrix[N*N] = {42, 40, 42, 88, 96, 99, 29, 57,		// 8x8
						  72, 54, 56, 89, 53, 75, 13, 15,
						   0, 42, 14,  9, 69, 28,  2, 59,
						  30, 69, 20,  4, 32, 79, 68, 70,
						  15, 20, 80, 17, 69, 10, 21, 10,
						   9, 88, 97, 88, 83, 45, 27, 41,
						  19,  3, 31, 10,  2, 91, 49, 69,
						  35, 67, 69, 42, 75, 29,  5, 41};

/**
 * Builds the identity matrix where the final result will
 * be written
 */
void buildIdentity(){
	for(int j=0; j<N; j++){
		for(int i=0; i<N; i++){
			if(i == j)
				*( (ptr + IDENTITY_OFFSET) + (j*N + i)) = 1;
			else
				*( (ptr + IDENTITY_OFFSET) + (j*N + i)) = 0;
		}
	}
}

/**
 * Copies the input matrix to RAM
 */
void storeInputMatrix(){
	for(int j=0; j<N; j++){
		for(int i=0; i<N; i++){
			*( (ptr + MATRIX_OFFSET) + (j*N + i)) = inputMatrix[j*N + i];
		}
	}
}

/**
 * Computes the inverse of the input matrix using
 * the Gauss-Jordan method
 */
void inverse(){
	int i,j,k;
	float temp;
	for(k=0; k<N; k++) {
		temp = *(ptr + MATRIX_OFFSET + (k*N + k));

		for(j=0; j<N; j++) {
			*(ptr + MATRIX_OFFSET + (k*N + j)) /= temp;
			*(ptr + IDENTITY_OFFSET + (k*N + j)) /= temp;
		}

		for(i=0; i<N; i++) {
			temp = *(ptr + MATRIX_OFFSET + (i*N + k));

			for(j=0; j<N; j++)	{
				if(i==k)
					break;
				*(ptr + MATRIX_OFFSET + (i*N + j)) -= *(ptr + MATRIX_OFFSET + (k*N + j)) * temp;
				*(ptr + IDENTITY_OFFSET + (i*N + j)) -= *(ptr + IDENTITY_OFFSET + (k*N + j)) * temp;
			}
		}
	}
}


void printMatrix(unsigned int pOffset){
	unsigned int *matPtr = (unsigned int*)ptr;
	for(int j=0; j<N; j++){
		for(int i=0; i<N; i++){
			alt_printf("%x ", *( (matPtr + pOffset) + (j*N + i)));
		}
		alt_putstr("\n");
	}
}



int main() {
	/*
	 * Reset (initialize to zero) all section counters and the global
	 * counter of the performance_counter peripheral.
	 */
	PERF_RESET (PERFORMANCE_COUNTER_BASE);


	alt_putstr("Executing...\n");

	/*
	 * Start the performance counter peripheral's global counter, thereby
	 * enabling all of the section counters of performance_counter.
	 */
	PERF_START_MEASURING (PERFORMANCE_COUNTER_BASE);

	//Build the identity matrix
	PERF_BEGIN (PERFORMANCE_COUNTER_BASE, IDENTITY_BUILD_SECTION);
	buildIdentity();
	PERF_END (PERFORMANCE_COUNTER_BASE, IDENTITY_BUILD_SECTION);

	//Store the input matrix in RAM
	PERF_BEGIN (PERFORMANCE_COUNTER_BASE, STORE_MATRIX_SECTION);
	storeInputMatrix();
	PERF_END (PERFORMANCE_COUNTER_BASE, STORE_MATRIX_SECTION);

	//Compute the inverse matrix
	PERF_BEGIN (PERFORMANCE_COUNTER_BASE, INVERSE_SECTION);
	inverse();
	PERF_END (PERFORMANCE_COUNTER_BASE, INVERSE_SECTION);

	alt_putstr("Printing result...\n");
	printMatrix(IDENTITY_OFFSET);

	//Stop the performance counter global counter
	PERF_STOP_MEASURING (PERFORMANCE_COUNTER_BASE);

	//Printf performance report
	perf_print_formatted_report((void*)PERFORMANCE_COUNTER_BASE, ALT_CPU_FREQ, 3, "identityMat", "storeMat", "inverse");

	alt_putstr("\n\nFinished\n");

  return 0;
}
