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
    Position pos = {-1, -1, '.'};
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


void move(int arr_side, char arr[arr_side][arr_side]) {
    Position p = find_karel(arr_side, arr);
    if (arr[p.x][p.y] == '^') {
        arr[p.x][p.y] = 'X';
        arr[p.x][p.y - 1] = '^';
    } else if (arr[p.x][p.y] == '>') {
        arr[p.x][p.y] = 'X';
        arr[p.x + 1][p.y] = '>';
    } else if (arr[p.x][p.y] == 'v') {
        arr[p.x][p.y] = 'X';
        arr[p.x][p.y + 1] = 'v';
    } else if (arr[p.x][p.y] == '<') {
        arr[p.x][p.y] = 'X';
        arr[p.x - 1][p.y] = '<';
    } else {
        fprintf(stderr, "Karel not found in move\n");
    }
}


void turn_right(int arr_side, char arr[arr_side][arr_side]) {
    Position p = find_karel(arr_side, arr);
    if (arr[p.x][p.y] == '^') {
        arr[p.x][p.y] = '>';
    } else if (arr[p.x][p.y] == '>') {
        arr[p.x][p.y] = 'v';
    } else if (arr[p.x][p.y] == 'v') {
        arr[p.x][p.y] = '<';
    } else if (arr[p.x][p.y] == '<') {
        arr[p.x][p.y] = '^';
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

int check_front(int arr_side, char arr[arr_side][arr_side]) {
    Position p = find_karel(arr_side, arr);
    if ((arr[p.x][p.y] == '^' && (arr[p.x][p.y - 1] == '#' || arr[p.x][p.y - 1] == 'O')) ||
        (arr[p.x][p.y] == '>' && (arr[p.x + 1][p.y] == '#' || arr[p.x + 1][p.y] == 'O')) ||
        (arr[p.x][p.y] == 'v' && (arr[p.x][p.y + 1] == '#' || arr[p.x][p.y + 1] == 'O')) ||
        (arr[p.x][p.y] == '<' && (arr[p.x - 1][p.y] == '#' || arr[p.x - 1][p.y] == 'O'))) {
        return 1;
    }
    return 0;
}


int check_front_x(int arr_side, char arr[arr_side][arr_side]) {
    Position p = find_karel(arr_side, arr);
    if ((arr[p.x][p.y] == '^' && arr[p.x][p.y - 1] == 'X') ||
        (arr[p.x][p.y] == '>' && arr[p.x + 1][p.y] == 'X') ||
        (arr[p.x][p.y] == 'v' && arr[p.x][p.y + 1] == 'X') ||
        (arr[p.x][p.y] == '<' && arr[p.x - 1][p.y] == 'X')) {
        return 1;
    }
    return 0;
}

int check_front_dir(int arr_side, char arr[arr_side][arr_side], char dir[arr_side][arr_side]) {
    Position p = find_karel(arr_side, arr);
    if ((dir[p.x][p.y] == '^' && dir[p.x][p.y - 1] == '^') ||
        (dir[p.x][p.y] == '>' && dir[p.x + 1][p.y] == '>') ||
        (dir[p.x][p.y] == 'v' && dir[p.x][p.y + 1] == 'v') ||
        (dir[p.x][p.y] == '<' && dir[p.x - 1][p.y] == '<')) {
        return 1;
    }
    return 0;
}

int check_boundary(int arr_side, char arr[arr_side][arr_side]) {
    Position p = find_karel(arr_side, arr);
    if ((arr[p.x][p.y] == '^' && p.y == 0) ||
        (arr[p.x][p.y] == '>' && p.x == arr_side - 1) ||
        (arr[p.x][p.y] == 'v' && p.y == arr_side - 1) ||
        (arr[p.x][p.y] == '<' && p.x == 0)) {
        //printf("p.x is %d and p.y is %d", p.x, p.y);
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


int karel_action(int arr_side, char arr[arr_side][arr_side], char dir[arr_side][arr_side]) {
    int blocked = 0;
    int boundary = 0;
    int count = 0;
    int limit = 1000;
    Position p = {-1, -1, '.'};
    while (boundary == 0 && count < limit) {
        boundary = check_boundary(arr_side, arr);

        if (boundary == 0) {
            blocked = check_front(arr_side, arr);
        } else {
            return 0;
        }

        p = find_karel(arr_side, arr);
        if (count == 0) {
            dir[p.x][p.y] = p.dir;
        }

        if (blocked == 1) {
            turn_right(arr_side, arr);
            p = find_karel(arr_side, arr);
            dir[p.x][p.y] = p.dir;
        } else {
            move(arr_side, arr);
            p = find_karel(arr_side, arr);
            dir[p.x][p.y] = p.dir;
        }

        count++;

        int in_cycle = check_front_dir(arr_side, arr, dir);
        if (in_cycle == 1) {
            printf("dir we are in a cycle\n");
            print_karel(arr_side, arr);
            print_karel(arr_side, dir);
            return 1;
        }
    }
    return 0;
}

int main() {
    char input[] = "....#..............#............#..............#.............#..^.............#.#...............#...";

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

    char dir_board[side][side];

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            dir_board[i][j] = '.';
        }
    }

    // what to do when you need to return > 1 thing from a function
    Position p = find_karel(side, board);
    printf("Karel is located at %d, %d, oriented %c", p.x, p.y, p.dir);
    printf("\n");

    int cycles = 0;
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            char tmp = board[i][j];
            if (tmp != '#' && tmp != '^' && tmp != '>' && tmp != 'v' && tmp != '<') {
                board[i][j] = 'O';
                cycles += karel_action(side, board, dir_board);
                memmove(board, orig_board, sizeof(orig_board));

                // reinitialize dir board
                for (int k = 0; k < side; k++) {
                    for (int l = 0; l < side; l++) {
                        dir_board[k][l] = '.';
                    }
                }

                board[i][j] = '.';
            }
        }
    }

    printf("%d", cycles);
    return 0;
}
