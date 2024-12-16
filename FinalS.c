#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int **dp;

char* read_string_from_file(const char* filename, int* length) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);  // Get the length of the file
    fseek(file, 0, SEEK_SET);

    if (*length == 0) {
        fclose(file);
        fprintf(stderr, "File is empty: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char* str = (char*)malloc((*length + 1) * sizeof(char));
    if (!str) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    fread(str, 1, *length, file);
    str[*length] = '\0';  // Null-terminate the string
    fclose(file);

    return str;
}

int LCS_length(char* X, char* Y, int lenX, int lenY) {
    // Allocate dp array dynamically
    dp = (int**)malloc((lenX + 1) * sizeof(int*));
    for (int i = 0; i <= lenX; i++) {
        dp[i] = (int*)malloc((lenY + 1) * sizeof(int));
    }

    // Initialize first row and column of dp matrix
    for (int i = 0; i <= lenX; i++) dp[i][0] = 0;
    for (int j = 0; j <= lenY; j++) dp[0][j] = 0;

    // Fill dp matrix
    for (int i = 1; i <= lenX; i++) {
        for (int j = 1; j <= lenY; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;  // Characters match
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }

    return dp[lenX][lenY];
}

void print_LCS(char* X, char* Y, int lenX, int lenY) {
    int index = dp[lenX][lenY];
    char lcs[index + 1];
    lcs[index] = '\0';

    int i = lenX, j = lenY;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[index - 1] = X[i - 1];
            i--;
            j--;
            index--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("LCS: %s\n", lcs);
}

double measure_execution_time(char* X, char* Y, int lenX, int lenY) {
    clock_t start = clock();

    LCS_length(X, Y, lenX, lenY);

    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    char filenameX[256], filenameY[256];
    printf("Enter filename for string X: ");
    scanf("%s", filenameX);
    printf("Enter filename for string Y: ");
    scanf("%s", filenameY);

    int lenX, lenY;
    char* X = read_string_from_file(filenameX, &lenX);
    char* Y = read_string_from_file(filenameY, &lenY);

    double exec_time = measure_execution_time(X, Y, lenX, lenY);
    printf("Execution time for LCS calculation: %.6f seconds\n", exec_time);

    int length = dp[lenX][lenY];
    printf("Length of LCS: %d\n", length);

    //print_LCS(X, Y, lenX, lenY);

    // Free dynamically allocated memory
    free(X);
    free(Y);
    for (int i = 0; i <= lenX; i++) {
        free(dp[i]);  // Free each row of dp
    }
    free(dp);  // Free the dp array

    return 0;
}
