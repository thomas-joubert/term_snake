#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"

void draw_board(int board[14][27])
{
    printf("\033[2J\033[1;1H");
    puts("-----------------------------");
    for (int i = 0; i < 14; i++)
    {
        printf("|");
        for (int j = 0; j < 27; j++)
        {
            switch(board[i][j])
            {
                case EMPTY:
                    printf(" ");
                    break;
                case CHERRY:
                    printf("*");
                    break;
                case BODY :
                    printf("o");
                    break;
                case HEAD :
                    printf("O");
                    break;
            }
        }
        printf("|\n");
    }
    puts("-----------------------------");
}

void init_board(int board[14][27])
{
    board[7][13] = HEAD;
    board[7][14] = BODY;
}

int move(int board[14][27], char direction, struct point *head)
{
    int alive = 1;
    switch (direction)
    {
        case UP:
            if ((alive = head->x == 0 ? 0 : 1))
            {
                board[head->x][head->y] = EMPTY;
                if (board[(head->x) - 1][head->y] == CHERRY)
                {
                    add_ring(board, *head, direction);
                    spawn_cherry(board);
                }
                if (board[head->x - 1][head->y] == BODY)
                    return 0;
                board[--(head->x)][head->y] = HEAD;
            }
            break;
        case DOWN:
            if ((alive = head->x == 13 ? 0 : 1))
            {
                board[head->x][head->y] = EMPTY;
                if (board[(head->x) + 1][head->y] == CHERRY)
                {
                    add_ring(board, *head, direction);
                    spawn_cherry(board);
                }
                if (board[head->x + 1][head->y] == BODY)
                    return 0;
                board[++(head->x)][head->y] = HEAD;
            }
            break;
        case RIGHT:
            if ((alive = head->y == 26 ? 0 : 1))
            {
                board[head->x][head->y] = EMPTY;
                if (board[head->x][(head->y) + 1] == CHERRY)
                {
                    add_ring(board, *head, direction);
                    spawn_cherry(board);
                }
                if (board[head->x][head->y + 1] == BODY)
                    return 0;
                board[head->x][++(head->y)] = HEAD;
            }
            break;
        case LEFT:
            if ((alive = head->y == 0 ? 0 : 1))
            {
                board[head->x][head->y] = EMPTY;
                if (board[head->x][(head->y) - 1] == CHERRY)
                {
                    add_ring(board, *head, direction);
                    spawn_cherry(board);
                }
                if (board[head->x][head->y - 1] == BODY)
                    return 0;
                board[head->x][--(head->y)] = HEAD;
            }
            break;
    }

    return alive;
}

void spawn_cherry(int board[14][27])
{
    srand(time(NULL));

    int row = rand() % 13;
    int column = rand() % 26;

    while(board[row][column] != EMPTY)
    {
        row = rand() % 13;
        column = rand() % 26;
    }

    board[row][column] = CHERRY;
}

void add_ring(int board[14][27], struct point head, char dir)
{
    int x_curr = head.x;
    int y_curr = head.y;

    int x_prev = x_curr;
    int y_prev = y_curr;

    char direction = dir;

    while(1)
    {
        if (board[x_curr + 1][y_curr] == BODY && x_curr + 1 != x_prev)
        {
            direction = DOWN;
            x_prev = x_curr++;
        }
        else if (board[x_curr - 1][y_curr] == BODY && x_curr - 1 != x_prev)
        {
            direction = UP;
            x_prev = x_curr--;
        }
        else if (board[x_curr][y_curr + 1] == BODY && y_curr + 1 != y_prev)
        {
            direction = RIGHT;
            y_prev = y_curr++;
        }
        else if (board[x_curr][y_curr - 1] == BODY && y_curr - 1 != y_prev)
        {
            direction = LEFT;
            y_prev = y_curr--;
        }
        else
        {
            if (direction == UP)
                board[x_curr - 1][y_curr] = BODY;
            else if (direction == DOWN)
                board[x_curr + 1][y_curr] = BODY;
            else if (direction == RIGHT)
                board[x_curr][y_curr + 1] = BODY;
            else if (direction == LEFT)
                board[x_curr][y_curr - 1] = BODY;

            break;
        }
    }
}
