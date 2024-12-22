#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *count_bin(int num, int size) {
    char *bin = (char *)malloc(size + 1);
    int index = 0;

    // build the string
    while (num != 0) {
        bin[index++] = (num % 2) + '0';
        num /= 2;
    }

    while (index < size) {
        bin[index++] = '0';
    }

    bin[index] = '\0';

    // reverse the string
    for (int i = 0, j = index - 1; i < j; i++, j--) {
        char tmp = bin[i]; // store the early element of the array
        bin[i] = bin[j]; // set the early element to the late element
        bin[j] = tmp; // set the late element to the early element
    }

    return bin;
}


int main() {
    char str[] = "3267: 81 40 27";

    char *token = strtok(str, " ");
    int str_len = sizeof(str)/sizeof(str[0]);
    int nums[str_len];
    int count = 0;
    while (token != NULL) {
        nums[count++] = atoi(token);
        token = strtok(NULL, " ");
    }

    // determine size of array
    int result_check = nums[0];

    // get rid of 0th element
    for (int i = 0; i < count; i++) {
        nums[i] = nums[i + 1];
    }

    // note that the array is larger than this, but we are only using the first specified elements. to trim the array we would use malloc

    // reduce the size of the array

    int num_size = count - 1; // dev
    int num_ops = num_size - 1;
    int bin_size = 1 << num_ops;

    // make the binary count
    char *bin_count[bin_size];

    for (int i = 0; i < bin_size; i++) {
        bin_count[i] = count_bin(i, num_ops);
    }

    // loop through the binary strings, called ops
    int match = 0;
    for (int i = 0; i < bin_size; i++) {
        int result = nums[0];
        char *ops = bin_count[i];

        for (int j = 1; j < num_size; j++) {
            if (ops[j - 1] == '0') {
                result *= nums[j];
            } else if (ops[j - 1] == '1') {
                result += nums[j];
            }
        }

        printf("%d is the result of %s operations\n", result, ops);

        if (result == result_check) {
            match = 1;
        }
    }

    printf("match has been set to %d", match);

    // free allocated memory
    for (int i = 0; i < bin_size; i++) {
        free(bin_count[i]);
    }

    return 0;
}
