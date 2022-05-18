#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char** argv) {
    int n=atoi(argv[1]); // This problem scales as n^3. 
                      // This value may need to be adjusted

    double* A = (double*)malloc(sizeof(double) * n * n);
    double* B = (double*)malloc(sizeof(double) * n * n);
    double* C = (double*)malloc(sizeof(double) * n * n);

    printf("Carrying out matrix-matrix multiplication\n");

    // Cilk Plus array notation
    int i, j , k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
             A[i * n + j] = (double)i / (double)n;
             B[i * n + j] = (double)j / (double)n;
        }
    }
    // Initially assign C [i] = 0
    for (i = 0; i < n; i++){        
        for (j = 0; j < n; j++){
           C[i * n + j] = 0;
        }
    }

    // C = A x B
    double time = omp_get_wtime();
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
    time = omp_get_wtime() - time;

    printf("Checking the results...\n");
    double norm = 0.0;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            norm += (C[i * n + j] - (double)(i * j) / (double)n) * (C[i * n + j] - (double)(i * j) / (double)n);

    if (norm > 1e-10)
        printf("Something is wrong... Norm is equal to %f\n", norm);
    else
        printf("Yup, we're good!\n");

    printf("Computing time: %f\n", time);
    return 0;
}
