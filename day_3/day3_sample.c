#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

int main() {
    const char *input = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
    const char *pattern = "mul\\([0-9]+,[0-9]+\\)";
    regex_t regex;
    regmatch_t match;
    int reti;

    // Compile the regex
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 1;
    }

    // Allocate memory for an array of substrings
    size_t match_capacity = 10; // Initial capacity for matches
    size_t match_count = 0;
    char **matches = malloc(match_capacity * sizeof(char *));
    if (!matches) {
        fprintf(stderr, "Memory allocation failed\n");
        regfree(&regex);
        return 1;
    }

    // Search the input string for matches
    const char *cursor = input;
    while (!regexec(&regex, cursor, 1, &match, 0)) {
        // Extract the match
        int start = match.rm_so;
        int end = match.rm_eo;
        int match_length = end - start;

        // Dynamically grow the array if needed
        if (match_count >= match_capacity) {
            match_capacity *= 2;
            char **new_matches = realloc(matches, match_capacity * sizeof(char *));
            if (!new_matches) {
                fprintf(stderr, "Memory reallocation failed\n");
                for (size_t i = 0; i < match_count; i++) {
                    free(matches[i]);
                }
                free(matches);
                regfree(&regex);
                return 1;
            }
            matches = new_matches;
        }

        // Allocate memory for the current match and copy it
        matches[match_count] = malloc(match_length + 1);
        if (!matches[match_count]) {
            fprintf(stderr, "Memory allocation failed for match\n");
            for (size_t i = 0; i < match_count; i++) {
                free(matches[i]);
            }
            free(matches);
            regfree(&regex);
            return 1;
        }
        strncpy(matches[match_count], cursor + start, match_length);
        matches[match_count][match_length] = '\0'; // Null-terminate the string
        match_count++;

        // Move the cursor forward
        cursor += end;
    }

    // Process each match
    printf("Results:\n");
    int sum = 0;
    for (size_t i = 0; i < match_count; i++) {
        int num1, num2;

        // Extract the two integers from the substring
        if (sscanf(matches[i], "mul(%d,%d)", &num1, &num2) == 2) {
            // Multiply the two integers
            int result = num1 * num2;
            sum += result;
            printf("%s -> %d\n", matches[i], result);
        } else {
            printf("Failed to extract integers from: %s\n", matches[i]);
        }

        free(matches[i]); // Free each match after processing
    }
    free(matches); // Free the array of pointers

    // Free the compiled regex
    regfree(&regex);
    printf("%d", sum);
    return 0;
}
