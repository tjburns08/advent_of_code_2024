#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Position;

Position find_karel(int arr_side, char arr[arr_side][arr_side]) {
    Position pos = {-1, -1};
    for (int i = 0; i < arr_side; i++) {
        for (int j = 0; j < arr_side; j++) {
            if (arr[i][j] == '>' || arr[i][j] == '^' || arr[i][j] == '<' || arr[i][j] == 'v') {
                pos.x = i;
                pos.y = j;
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
    if ((arr[p.x][p.y] == '^' && arr[p.x][p.y - 1] == '#') ||
        (arr[p.x][p.y] == '>' && arr[p.x + 1][p.y] == '#') ||
        (arr[p.x][p.y] == 'v' && arr[p.x][p.y + 1] == '#') ||
        (arr[p.x][p.y] == '<' && arr[p.x - 1][p.y] == '#')) {
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

    // what to do when you need to return > 1 thing from a function
    Position p = find_karel(side, board);
    printf("Karel is located at %d, %d", p.x, p.y);
    printf("\n");
    //print_karel(side, board);

    int blocked = 0;
    int boundary = 0;
    while (boundary == 0) {
        blocked = check_front(side, board);
        boundary = check_boundary(side, board);
        if (blocked == 1) {
            turn_right(side, board);
        }
        move(side, board);
        //print_karel(side, board);
    }

    int result = count_x(side, board);
    printf("%d", result);
    return 0;
}
