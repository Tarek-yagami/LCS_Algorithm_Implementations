#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// CUDA Kernel for LCS computation
__global__ void compute_lcs(char *X, char *Y, int *C, int m, int n, int k) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = k - i;

    if (i <= m && j >= 0 && j <= n) {
        if (i == 0 || j == 0) {
            C[i * (n + 1) + j] = 0;
        } else if (X[i - 1] == Y[j - 1]) {
            C[i * (n + 1) + j] = C[(i - 1) * (n + 1) + (j - 1)] + 1;
        } else {
            C[i * (n + 1) + j] = MAX(C[(i - 1) * (n + 1) + j], C[i * (n + 1) + (j - 1)]);
        }
    }
}

// Function to read a string from a file
char *read_string_from_file(const char *filename, int *length) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *str = (char *)malloc((*length + 1) * sizeof(char));
    fread(str, 1, *length, file);
    str[*length] = '\0';
    fclose(file);
    return str;
}

int main() {
    int m, n;
    char *X, *Y;
    char filenameX[256], filenameY[256];

    printf("Enter filename for string X: ");
    scanf("%s", filenameX);
    printf("Enter filename for string Y: ");
    scanf("%s", filenameY);

    X = read_string_from_file(filenameX, &m);
    Y = read_string_from_file(filenameY, &n);

    // Allocate memory on host
    int *C = (int *)malloc((m + 1) * (n + 1) * sizeof(int));

    // Allocate memory on device
    char *d_X, *d_Y;
    int *d_C;
    cudaMalloc((void **)&d_X, m * sizeof(char));
    cudaMalloc((void **)&d_Y, n * sizeof(char));
    cudaMalloc((void **)&d_C, (m + 1) * (n + 1) * sizeof(int));

    // Copy data to device
    cudaMemcpy(d_X, X, m * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_Y, Y, n * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemset(d_C, 0, (m + 1) * (n + 1) * sizeof(int));

    dim3 threadsPerBlock(256);
    dim3 blocksPerGrid((m + threadsPerBlock.x - 1) / threadsPerBlock.x);

    // Measure execution time
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    for (int k = 0; k <= m + n; ++k) {
        compute_lcs<<<blocksPerGrid, threadsPerBlock>>>(d_X, d_Y, d_C, m, n, k);
        cudaDeviceSynchronize();
    }

    // Measure stop time
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    float time;
    cudaEventElapsedTime(&time, start, stop);

    printf("Execution Time: %f ms\n", time);

    // Copy result back to host
    cudaMemcpy(C, d_C, (m + 1) * (n + 1) * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Length of LCS: %d\n", C[m * (n + 1) + n]);

    // Free memory
    free(X);
    free(Y);
    free(C);
    cudaFree(d_X);
    cudaFree(d_Y);
    cudaFree(d_C);

    return 0;
}