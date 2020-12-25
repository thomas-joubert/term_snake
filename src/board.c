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

struct point move_body(int board[14][27], struct point head, char direction)
{
    int x_curr = head.x;
    int y_curr = head.y;

    int x_prev = x_curr;
    int y_prev = y_curr;

    while(1)
    {
        if (x_curr < 13 && board[x_curr + 1][y_curr] == BODY && direction != DOWN)
        {
            direction = UP;
            x_prev = x_curr++;
        }
        else if (x_curr > 0 && board[x_curr - 1][y_curr] == BODY && direction != UP)
        {
            direction = DOWN;
            x_prev = x_curr--;
        }
        else if (y_curr < 26 && board[x_curr][y_curr + 1] == BODY && direction != RIGHT)
        {
            direction = LEFT;
            y_prev = y_curr++;
        }
        else if (y_curr > 0 && board[x_curr][y_curr - 1] == BODY && direction != LEFT)
        {
            direction = RIGHT;
            y_prev = y_curr--;
        }
        else
            break;

        board[x_curr][y_curr] = board[x_prev][y_prev];
    }

    board[x_curr][y_curr] = EMPTY;

    struct point tail;
    tail.x = x_curr;
    tail.y = y_curr;

    return tail;
}

int move(int board[14][27], char direction, struct point *head)
{
    int alive = 1;
    int cherry = 0;

    struct point tail;

    switch (direction)
    {
        case UP:
            if ((alive = head->x == 0 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry = board[head->x - 1][head->y] == CHERRY;

                if (board[head->x - 1][head->y] == BODY)
                    return 0;

                board[head->x - 1][head->y] = HEAD;

                tail = move_body(board, *head, direction);
                if (cherry)
                {
                    add_ring(board, tail);
                    spawn_cherry(board);
                }

                head->x--;
            }
            break;
        case DOWN:
            if ((alive = head->x == 13 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry = board[head->x + 1][head->y] == CHERRY;

                if (board[head->x + 1][head->y] == BODY)
                    return 0;

                board[head->x + 1][head->y] = HEAD;

                tail = move_body(board, *head, direction);

                if (cherry)
                {
                    add_ring(board, tail);
                    spawn_cherry(board);
                }
                head->x++;
            }
            break;
        case RIGHT:
            if ((alive = head->y == 26 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry = board[head->x][head->y + 1] == CHERRY;

                if (board[head->x][head->y + 1] == BODY)
                    return 0;

                board[head->x][head->y + 1] = HEAD;

                tail = move_body(board, *head, direction);

                if (cherry)
                {
                    add_ring(board, tail);
                    spawn_cherry(board);
                }
                head->y++;
            }
            break;
        case LEFT:
            if ((alive = head->y == 0 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry = board[head->x][head->y - 1] == CHERRY;

                if (board[head->x][head->y - 1] == BODY)
                    return 0;

                board[head->x][head->y - 1] = HEAD;

                tail = move_body(board, *head, direction);

                if (cherry)
                {
                    add_ring(board, tail);
                    spawn_cherry(board);
                }
                head->y--;
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


// TODO : Handle when at stick a the side of the board
void add_ring(int board[14][27], struct point tail)
{
    board[tail.x][tail.y] = BODY;
}
