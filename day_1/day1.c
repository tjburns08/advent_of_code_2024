#include <stdio.h>
#include <string.h>

// needed for qsort
int comp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
 
int main() {
    // read in the file
    FILE *fptr;
    fptr = fopen("puzzle_input.txt", "r");

    // loop through each line
    // isolate the values on the left and the right side
    // size of file is 1000 lines, so we don't have to dynamically allocate
    char contents[100];
    int count = 0;
    char arr[1000][15]; // 13, a newline, and a null terminator?

    int left[1000]; // column 1 of the list as an array
    int right[1000]; // column 2 of the list as an array
    while (fgets(contents, 100, fptr)) {
        // 5 chars, 3 whitespace, 5 other chars
        strcpy(arr[count], contents);

        // slice out the first 5 chars
        int curr_left[5];
        int curr_right[5];
        for (int i = 0; i < 5; i++) {
            curr_left[i] =  arr[count][i] - '0';
            curr_right[i] = arr[count][i + 8] - '0';
        }

        // convert array of ints to an int
        int tmp = 0;
        for (int i = 0; i < 5; i++) {
            tmp = tmp*10 + curr_left[i];
        }

        left[count] = tmp;

        tmp = 0;

        for (int i = 0; i < 5; i++) {
            tmp = tmp*10 + curr_right[i];
        }

        right[count] = tmp;

        count++;
    }

    fclose(fptr);

    // sort the arrays
    int n = sizeof(left)/sizeof(left[0]);
    qsort(left, n, sizeof(left[0]), comp);
    qsort(right, n, sizeof(right[0]), comp);

    // add up the distances
    int result = 0;
    for (int i = 0; i < 1000; i++) {
        int curr = left[i] - right[i];
        if (curr < 0) {
            curr = -curr;
        }

        result += curr;
    }

    // part 1
    printf("%d\n", result);

    // part 2
    int sim_score = 0;
    for (int i = 0; i < 1000; i++) {

        int right_occur = 0;
        for (int j = 0; j < 1000; j++) {
            if (left[i] == right[j]) {
                right_occur++;
            }
        }
        sim_score += left[i] * right_occur;
    }

    printf("%d\n", sim_score);

    return 0;
}
