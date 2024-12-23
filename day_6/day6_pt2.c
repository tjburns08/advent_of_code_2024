#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 150
#define MAX_COLS 150

typedef struct {
    int row;
    int col;
    char dir;
} Position;

void clean_line(char *line, size_t max_len) {
    for (size_t i = 0; i < max_len; i++) {
        if (line[i] == '\0') {
            line[i] = '.';  // Replace unwanted nulls or any character of your choice
        }
    }
}

void read_file(FILE *fptr, char lines[MAX_ROWS][MAX_COLS], int *num_lines) {
    *num_lines = 0;

    while (*num_lines < MAX_ROWS && fgets(lines[*num_lines], MAX_COLS, fptr)) {
        size_t len = strlen(lines[*num_lines]);
        if (len > 0 && lines[*num_lines][len - 1] == '\n') {
            lines[*num_lines][len - 1] = '\0';
        }

        clean_line(lines[*num_lines], MAX_COLS);

        (*num_lines)++;
    }
}

Position find_karel(int arr_side, char arr[arr_side][arr_side]) {
    Position pos = {-1, -1};
    for (int i = 0; i < arr_side; i++) {
        for (int j = 0; j < arr_side; j++) {
            if (arr[i][j] == '>' || arr[i][j] == '^' || arr[i][j] == '<' || arr[i][j] == 'v') {
                pos.row = i;
                pos.col = j;
                pos.dir = arr[i][j];
                return pos;
            }
        }
    }
    return pos;
}

void move(int arr_side, char arr[arr_side][arr_side], Position *p) {
    if (arr[p->row][p->col] == '^') {
        arr[p->row][p->col] = 'X';
        p->row -= 1;
        arr[p->row][p->col] = '^';
    } else if (arr[p->row][p->col] == '>') {
        arr[p->row][p->col] = 'X';
        p->col += 1;
        arr[p->row][p->col] = '>';
    } else if (arr[p->row][p->col] == 'v') {
        arr[p->row][p->col] = 'X';
        p->row += 1;
        arr[p->row][p->col] = 'v';
    } else if (arr[p->row][p->col] == '<') {
        arr[p->row][p->col] = 'X';
        p->col -= 1;
        arr[p->row][p->col] = '<';
    } else {
        fprintf(stderr, "Karel not found in move\n");
    }
    // printf("move has been executed\n");
}


void turn_right(int arr_side, char arr[arr_side][arr_side], Position *p) {
    if (arr[p->row][p->col] == '^') {
        arr[p->row][p->col] = '>';
        p->dir = '>';
    } else if (arr[p->row][p->col] == '>') {
        arr[p->row][p->col] = 'v';
        p->dir = 'v';
    } else if (arr[p->row][p->col] == 'v') {
        arr[p->row][p->col] = '<';
        p->dir = '<';
    } else if (arr[p->row][p->col] == '<') {
        arr[p->row][p->col] = '^';
        p->dir = '^';
    } else {
        fprintf(stderr, "Karel not found in turn_right\n");
    }
}

void print_karel(int arr_side, char arr[arr_side][arr_side]) {
    for (int i = 0; i < arr_side; i++) {
        for (int j = 0; j < arr_side; j++) {
            printf("%c", arr[i][j]); // gives us the coordinate system that we want
        }
        printf("\n");
    }
    printf("\n");
}

int check_front(int arr_side, char arr[arr_side][arr_side], Position p) {
    if ((arr[p.row][p.col] == '^' && (arr[p.row - 1][p.col] == '#' || arr[p.row - 1][p.col] == 'O')) ||
        (arr[p.row][p.col] == '>' && (arr[p.row][p.col + 1] == '#' || arr[p.row][p.col + 1] == 'O')) ||
        (arr[p.row][p.col] == 'v' && (arr[p.row + 1][p.col] == '#' || arr[p.row + 1][p.col] == 'O')) ||
        (arr[p.row][p.col] == '<' && (arr[p.row][p.col - 1] == '#' || arr[p.row][p.col - 1] == 'O'))) {
        return 1;
    }
    return 0;
}

int check_front_dir(int arr_side, char dir[arr_side][arr_side], Position p) {
    if ((dir[p.row][p.col] == '^' && dir[p.row - 1][p.col] == '^') ||
        (dir[p.row][p.col] == '>' && dir[p.row][p.col + 1] == '>') ||
        (dir[p.row][p.col] == 'v' && dir[p.row + 1][p.col] == 'v') ||
        (dir[p.row][p.col] == '<' && dir[p.row][p.col - 1] == '<')) {
        return 1;
    }
    return 0;
}

int check_boundary(int arr_side, char arr[arr_side][arr_side], Position p) {
    if ((arr[p.row][p.col] == '^' && p.row == 0) ||
        (arr[p.row][p.col] == '>' && p.col == arr_side - 1) ||
        (arr[p.row][p.col] == 'v' && p.row == arr_side - 1) ||
        (arr[p.row][p.col] == '<' && p.col == 0)) {
        // printf("We are at a boundary\n");
        return 1;
    }
    return 0;
}

int count_x(int arr_side, char arr[arr_side][arr_side]) {
    int count = 0;
    for (int i = 0; i < arr_side; i++) {
        for (int j = 0; j < arr_side; j++) {
            if (arr[i][j] == 'X') {
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
    int limit = 100000;
    while (boundary == 0 && count <= limit) {
        // print_karel(arr_side, arr);
        // print_karel(arr_side, dir);
        boundary = check_boundary(arr_side, arr, *p);

        if (boundary == 0) {
            blocked = check_front(arr_side, arr, *p);
        } else {
            return 0;
        }

        if (count == 0) {
            dir[p->row][p->col] = p->dir;
        }

        if (blocked == 1) {
            turn_right(arr_side, arr, p);
            // printf("Karel turned right and is now facing %c\n", p->dir);
            dir[p->row][p->col] = p->dir;
        } else {
            move(arr_side, arr, p);
            dir[p->row][p->col] = p->dir;
        }

        count++;

        if (check_front_dir(arr_side, dir, *p) == 1 || count == limit) {
            //printf("we are in a cycle\n");
            //print_karel(arr_side, arr);
            //print_karel(arr_side, dir);
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

    char lines[MAX_ROWS][MAX_COLS];
    int num_lines;

    read_file(fptr, lines, &num_lines);

    char orig_board[num_lines][num_lines];
    char dir_board[num_lines][num_lines];
    char board[num_lines][num_lines];

    for (int i = 0; i < num_lines; i++) {
        for (int j = 0; j < num_lines; j++) {
            board[i][j] = '.';
            orig_board[i][j] = '.';
            dir_board[i][j] = '.';
        }
    }

    // a tmp board for resetting
    for (int i = 0; i < num_lines; i++) {
        for (int j = 0; j < num_lines; j++) {
            board[i][j] = lines[i][j];
            orig_board[i][j] = board[i][j];
            // dir_board[i][j] = '.';
        }
    }


    // Print the boards
    printf("Initial Board:\n");
    print_karel(num_lines, board);

    printf("Original Board:\n");
    print_karel(num_lines, orig_board);

    printf("Direction Board:\n");
    print_karel(num_lines, dir_board);

    // what to do when you need to return > 1 thing from a function
    Position p = find_karel(num_lines, board);
    Position p_orig = p;
    printf("Karel is located at row %d, col %d facing %c", p.row, p.col, p.dir);
    printf("\n");

    int cycles = 0;
    int test_count = 0;
    for (int i = 0; i < num_lines; i++) { // set to num_lines for full, 1 or 2 for debug
        for (int j = 0; j < num_lines; j++) {
            // printf("We are in iter %d of %d, where i = %d and j = %d\n", test_count, num_lines, i, j);
            char tmp = board[i][j];
            if (tmp != '#' && tmp != '^' && tmp != '>' && tmp != 'v' && tmp != '<') {
                board[i][j] = 'O';
                cycles = cycles + karel_action(num_lines, board, dir_board, &p);
                // printf("Karel is located at row %d col %d) facing %c\n", p.row, p.col, p.dir);
                //print_karel(num_lines, dir_board); // debug
                memcpy(board, orig_board, sizeof(orig_board));

                // reinitialize the position
                p = p_orig;

                // reinitialize the dir board
                for (int k = 0; k < num_lines; k++) {
                    for (int l = 0; l < num_lines; l++) {
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
    return 0;
}
