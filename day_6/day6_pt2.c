#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
    char dir;
} Position;

Position find_karel(int arr_side, char arr[arr_side][arr_side]) {
    Position pos = {-1, -1};
    for (int i = 0; i < arr_side; i++) {
        for (int j = 0; j < arr_side; j++) {
            if (arr[i][j] == '>' || arr[i][j] == '^' || arr[i][j] == '<' || arr[i][j] == 'v') {
                pos.x = i;
                pos.y = j;
                pos.dir = arr[i][j];
                return pos;
            }
        }
    }
    return pos;
}

void move(int arr_side, char arr[arr_side][arr_side], Position *p) {
    if (arr[p->x][p->y] == '^') {
        arr[p->x][p->y] = 'X';
        p->y -= 1;
        arr[p->x][p->y] = '^';
    } else if (arr[p->x][p->y] == '>') {
        arr[p->x][p->y] = 'X';
        p->x += 1;
        arr[p->x][p->y] = '>';
    } else if (arr[p->x][p->y] == 'v') {
        arr[p->x][p->y] = 'X';
        p->y += 1;
        arr[p->x][p->y] = 'v';
    } else if (arr[p->x][p->y] == '<') {
        arr[p->x][p->y] = 'X';
        p->x -= 1;
        arr[p->x][p->y] = '<';
    } else {
        fprintf(stderr, "Karel not found in move\n");
    }
    // printf("move has been executed\n");
}


void turn_right(int arr_side, char arr[arr_side][arr_side], Position *p) {
    if (arr[p->x][p->y] == '^') {
        arr[p->x][p->y] = '>';
        p->dir = '>';
    } else if (arr[p->x][p->y] == '>') {
        arr[p->x][p->y] = 'v';
        p->dir = 'v';
    } else if (arr[p->x][p->y] == 'v') {
        arr[p->x][p->y] = '<';
        p->dir = '<';
    } else if (arr[p->x][p->y] == '<') {
        arr[p->x][p->y] = '^';
        p->dir = '^';
    } else {
        fprintf(stderr, "Karel not found in turn_right\n");
    }
}

void print_karel(int arr_side, char arr[arr_side][arr_side]) {
    for (int i = 0; i < arr_side; i++) {
        for (int j = 0; j < arr_side; j++) {
            printf("%c", arr[j][i]); // gives us the coordinate system that we want
        }
        printf("\n");
    }
    printf("\n");
}

int check_front(int arr_side, char arr[arr_side][arr_side], Position p) {
    if ((arr[p.x][p.y] == '^' && (arr[p.x][p.y - 1] == '#' || arr[p.x][p.y - 1] == 'O')) ||
        (arr[p.x][p.y] == '>' && (arr[p.x + 1][p.y] == '#' || arr[p.x + 1][p.y] == 'O')) ||
        (arr[p.x][p.y] == 'v' && (arr[p.x][p.y + 1] == '#' || arr[p.x][p.y + 1] == 'O')) ||
        (arr[p.x][p.y] == '<' && (arr[p.x - 1][p.y] == '#' || arr[p.x - 1][p.y] == 'O'))) {
        return 1;
    }
    return 0;
}

int check_front_dir(int arr_side, char dir[arr_side][arr_side], Position p) {
    if ((dir[p.x][p.y] == '^' && dir[p.x][p.y - 1] == '^') ||
        (dir[p.x][p.y] == '>' && dir[p.x + 1][p.y] == '>') ||
        (dir[p.x][p.y] == 'v' && dir[p.x][p.y + 1] == 'v') ||
        (dir[p.x][p.y] == '<' && dir[p.x - 1][p.y] == '<')) {
        return 1;
    }
    return 0;
}

int check_boundary(int arr_side, char arr[arr_side][arr_side], Position p) {
    if ((arr[p.x][p.y] == '^' && p.y == 0) ||
        (arr[p.x][p.y] == '>' && p.x == arr_side - 1) ||
        (arr[p.x][p.y] == 'v' && p.y == arr_side - 1) ||
        (arr[p.x][p.y] == '<' && p.x == 0)) {
        printf("We are at a boundary\n");
        return 1;
    }
    return 0;
}

int count_x(int arr_side, char arr[arr_side][arr_side]) {
    int count = 0;
    for (int i = 0; i < arr_side; i++) {
        for (int j = 0; j < arr_side; j++) {
            if (arr[j][i] == 'X') {
                count++;
            }
        }
    }
    return count;
}

int karel_action(int arr_side, char arr[arr_side][arr_side], char dir[arr_side][arr_side], Position *p) {
    int blocked = 0;
    int boundary = 0;
    int count = 0;
    int limit = 10000;
    while (boundary == 0 && count < limit) {
        print_karel(arr_side, arr);
        print_karel(arr_side, dir);
        boundary = check_boundary(arr_side, arr, *p);

        if (boundary == 0) {
            blocked = check_front(arr_side, arr, *p);
        } else {
            return 0;
        }

        if (count == 0) {
            dir[p->x][p->y] = p->dir;
        }

        if (blocked == 1) {
            turn_right(arr_side, arr, p);
            printf("Karel turned right and is now facing %c\n", p->dir);
            dir[p->x][p->y] = p->dir;
        } else {
            move(arr_side, arr, p);
            dir[p->x][p->y] = p->dir;
        }

        count++;

        if (check_front_dir(arr_side, dir, *p) == 1) {
            printf("we are in a cycle\n");
            // print_karel(arr_side, arr);
            // print_karel(arr_side, dir);
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE *fptr;
    fptr = fopen("input.txt", "r"); // change to match directory
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    int rows = 130;
    int cols = 130;

    int size = rows * cols;
    char *input = malloc(size * sizeof(char));
    if (input == NULL) {
        fprintf(stderr, "Memory allocation failed");
        fclose(fptr);
        return 1;
    }

    // Read in a file line by line
    int count = 0;
    int ch;
    while ((ch = fgetc(fptr)) != EOF && count < size) {
        if (ch != '\n') {
            input[count++] = (char)ch;
        }
    }

    if (count != size) {
        fprintf(stderr, "Warning: expected %d characters, but read %d\n", size, count);
    }

    // assumes the board is a perfect square
    int input_size = strlen(input);
    int side = sqrt(input_size);
    char board[side][side];
    int count2 = 0;

    // Makes x-i go across and y-j go down, with the origin at top left
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            board[j][i] = input[count2];
            count2++;
        }
    }

    // a tmp board for resetting
    char orig_board[side][side];
    memcpy(orig_board, board, sizeof(board));

    // a board for recording direction
    char dir_board[side][side];

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            dir_board[i][j] = '.';
        }
    }

    // what to do when you need to return > 1 thing from a function
    Position p = find_karel(side, board);
    Position p_orig = p;
    printf("Karel is located at (%d, %d) facing %c", p.x, p.y, p.dir);
    printf("\n");

    int cycles = 0;
    int test_count = 0;
    for (int i = 11; i < 12; i++) { // set to side for full, 1 or 2 for debug
        for (int j = 42; j < 43; j++) {
            printf("We are in iter %d of %d, where i = %d and j = %d\n", test_count, size, i, j);
            char tmp = board[i][j];
            if (tmp != '#' && tmp != '^' && tmp != '>' && tmp != 'v' && tmp != '<') {
                board[i][j] = 'O';
                cycles = cycles + karel_action(side, board, dir_board, &p);
                printf("Karel is located at (%d, %d) facing %c\n", p.x, p.y, p.dir);
                // print_karel(side, dir_board); // debug
                memcpy(board, orig_board, sizeof(orig_board));

                // reinitialize the position
                p = p_orig;

                // reinitialize the dir board
                for (int k = 0; k < side; k++) {
                    for (int l = 0; l < side; l++) {
                        dir_board[k][l] = '.';
                    }
                }

                board[i][j] = '.';
            }
            test_count++;
        }
    }

    printf("%d", cycles);

    fclose(fptr);
    free(input);
    return 0;
}
