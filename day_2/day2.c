#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
    // open the file
    FILE *fptr;
    char *filename = "puzzle_input.txt";
    fptr = fopen(filename, "r");

    if(!fptr) {
        fprintf(stderr, "could not open file");
        return 1;
    }

    // convert the file into an array
    char buffer[100];
    int count = 0;
    char arr[1000][100];

    while(fgets(buffer, sizeof(buffer), fptr)) {
        strcpy(arr[count], buffer);
        count++;

        if (count > 1000) {
            printf("reached maximum number of lines");
        }
    }

    fclose(fptr);

    // convert the char array to int array
    for (int i = 0; i < 1000; i++) {
        int prev = -1;
        int next = -1;
        int result[100];
        int count = 0;
        for (int j = 0; j < (int)sizeof(arr[i]) - 1; j++) {
            int curr = arr[i][j] - '0';

            if (j > 0) {
                prev = arr[i][j - 1] - '0';
            }

            if (j < (int)sizeof(arr[i]) - 2) {
                next = arr[i][j + 1] - '0';
            } else {
                next = -1;
            }

            if (curr > 0 && prev > 0) {
                result[count] = 10 * prev + curr;
                count++;
            } else if (curr > 0 && prev < 0 && next < 0) {
                result[count] = curr;
                count++;
            }
        }

        // are we increasing or decreasing
        for (int i = 0; i < 100; i++) {
            if(result[i] != 0) {
                //printf("%d\n", result[i]);
            }
            printf("%d\n", result[i]);
        }
        //printf("\n");
    }

    return 0;
}
