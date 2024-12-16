#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h> // Include the time library

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// Define LCS matrix dimensions
int m, n;
char *X, *Y;
int **C;
pthread_barrier_t barrier;
int NUM_THREADS;  // Number of threads will be set by the user

void *compute_lcs(void *arg) {
    int thread_id = *(int *)arg;

    for (int k = 0; k <= m + n; ++k) {
        // Each thread processes its own set of cells along the anti-diagonal
        for (int i = thread_id; i <= m; i += NUM_THREADS) {
            int j = k - i;
            if (j >= 0 && j <= n) {
                if (i == 0 || j == 0) {
                    C[i][j] = 0; // Base case: LCS length is 0 if one string is empty
                } else if (X[i - 1] == Y[j - 1]) {
                    C[i][j] = C[i - 1][j - 1] + 1; // Characters match
                } else {
                    C[i][j] = MAX(C[i - 1][j], C[i][j - 1]); // No match
                }
            }
        }
        // Synchronize threads after each anti-diagonal
        pthread_barrier_wait(&barrier);
    }
    
    pthread_exit(NULL);
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

// Function to print the LCS string
void print_lcs() {
    int i = m, j = n;
    int lcs_length = C[m][n];
    char lcs[lcs_length + 1];
    lcs[lcs_length] = '\0';  // Null-terminate the LCS string

    // Backtrack from C[m][n] to construct the LCS string
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[--lcs_length] = X[i - 1];  // Add character to LCS
            i--;
            j--;
        } else if (C[i - 1][j] > C[i][j - 1]) {
            i--; // Move up
        } else {
            j--; // Move left
        }
    }

    printf("LCS: %s\n", lcs);
}

int main() {
    // Get user input for the number of threads
    printf("Enter the number of threads: ");
    scanf("%d", &NUM_THREADS);

    char filenameX[256], filenameY[256];
    printf("Enter filename for string X: ");
    scanf("%s", filenameX);
    printf("Enter filename for string Y: ");
    scanf("%s", filenameY);

    X = read_string_from_file(filenameX, &m);
    Y = read_string_from_file(filenameY, &n);

    // Calculate the lengths of the strings
    m = strlen(X);
    n = strlen(Y);

    // Initialize the LCS matrix
    C = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        C[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    // Initialize barrier with NUM_THREADS participants
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Start timing the execution
    clock_t start_time = clock();

    // Create threads
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int t = 0; t < NUM_THREADS; ++t) {
        thread_ids[t] = t;
        pthread_create(&threads[t], NULL, compute_lcs, (void *)&thread_ids[t]);
    }

    // Join threads
    for (int t = 0; t < NUM_THREADS; ++t) {
        pthread_join(threads[t], NULL);
    }

    // Stop timing the execution
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Calculate time taken

    // Print execution time
    printf("Execution Time: %f seconds\n", time_taken);
    
    // Output LCS length and print the LCS string
    printf("Length of LCS: %d\n", C[m][n]);
    //print_lcs();
    
    

    // Clean up
    pthread_barrier_destroy(&barrier);
    for (int i = 0; i <= m; i++) {
        free(C[i]);
    }
    free(C);
    free(X);
    free(Y);

    return 0;
}
