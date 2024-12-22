#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input[] = "75,47,61,53,29";
    int numbers[10];
    int count = 0;

    // convert array of strings into integers split on comma
    char *token = strtok(input, ",");
    while (token != NULL) {
        numbers[count++] = atoi(token);
        token = strtok(NULL, ",");
    }

    // print check
    for (int i = 0; i < count; i++) {
        //printf("%d\n", numbers[i]);
    }

    // code in the rule 75|53 (does not exist in rule set)
    char rule[] = "75|53";

    // tokenize as before
    int rule_nums[2];
    token = strtok(rule, "|");
    int count2 = 0;
    while (token != NULL) {
        rule_nums[count2++] = atoi(token);
        token = strtok(NULL, "|");
    }

    // print the elements of rule
    printf("%d\n", rule_nums[0]);
    printf("%d\n", rule_nums[1]);

    // check the rule. found will equal 2 if the rule is satisfied
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] == rule_nums[0] || numbers[i] == rule_nums[1]) {
            found++;
        }
    }

    printf("%d\n", found);

    // check if 75 does in fact come before 53
    if (found == 2) {
        // report the middle number of the array
        printf("%d\n", numbers[count/2]);
    }


    return 0;
}
