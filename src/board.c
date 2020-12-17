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
                case PLAYER :
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
                board[--(head->x)][head->y] = HEAD;
            }
            break;
        case DOWN:
            if ((alive = head->x == 13 ? 0 : 1))
            {
                board[head->x][head->y] = EMPTY;
                board[++(head->x)][head->y] = HEAD;
            }
            break;
        case RIGHT:
            if ((alive = head->y == 26 ? 0 : 1))
            {
                board[head->x][head->y] = EMPTY;
                board[head->x][++(head->y)] = HEAD;
            }
            break;
        case LEFT:
            if ((alive = head->y == 0 ? 0 : 1))
            {
                board[head->x][head->y] = EMPTY;
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
