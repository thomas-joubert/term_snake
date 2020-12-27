#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "snake.h"
#include "score.h"

static void reset(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = 0;
    }
}

static void update_board(int board[BOARD_SIZE][BOARD_SIZE], struct body *head, struct point *cherry)
{
    reset(board);

    board[head->x][head->y] = HEAD;

    struct body *curr = head->next;

    while(curr)
    {
        board[curr->x][curr->y] = BODY;
        curr = curr->next;
    }

    board[cherry->x][cherry->y] = CHERRY;
}

void draw_board(int board[BOARD_SIZE][BOARD_SIZE], struct body *head, struct point *cherry)
{
    update_board(board, head, cherry);

    printf("\033[2J\033[1;1H");
    puts("-------------------");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("|");
        for (int j = 0; j < BOARD_SIZE; j++)
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
    puts("-------------------");
    print_score();
}

void move_body(struct body *head, char direction)
{
    int inc_x = 0;
    int inc_y = 0;

    switch (direction)
    {
        case UP:
            inc_x = -1;
            break;
        case DOWN:
            inc_x = 1;
            break;
        case LEFT:
            inc_y = -1;
            break;
        case RIGHT:
            inc_y = 1;
            break;
    }

    int x_curr = head->x;
    int y_curr = head->y;
    struct body *curr = head->next;
    while(curr)
    {
        int x_tmp = curr->x;
        int y_tmp = curr->y;

        curr->x = x_curr;
        curr->y = y_curr;

        x_curr = x_tmp;
        y_curr = y_tmp;

        curr = curr->next;
    }

    head->x += inc_x;
    head->y += inc_y;
}

int move(int board[BOARD_SIZE][BOARD_SIZE], char direction, struct body *head, struct point *cherry)
{
    int alive = 1;
    int cherry_check = 0;

    switch (direction)
    {
        case UP:
            if ((alive = head->x == 0 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry_check = board[head->x - 1][head->y] == CHERRY;

                if (board[head->x - 1][head->y] == BODY)
                    return 0;

                board[head->x - 1][head->y] = HEAD;

                move_body(head, direction);
            }
            break;
        case DOWN:
            if ((alive = head->x == 16 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry_check = board[head->x + 1][head->y] == CHERRY;

                if (board[head->x + 1][head->y] == BODY)
                    return 0;

                board[head->x + 1][head->y] = HEAD;

                move_body(head, direction);
            }
            break;
        case RIGHT:
            if ((alive = head->y == 16 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry_check = board[head->x][head->y + 1] == CHERRY;

                if (board[head->x][head->y + 1] == BODY)
                    return 0;

                board[head->x][head->y + 1] = HEAD;

                move_body(head, direction);
            }
            break;
        case LEFT:
            if ((alive = head->y == 0 ? 0 : 1))
            {
                board[head->x][head->y] = BODY;
                cherry_check = board[head->x][head->y - 1] == CHERRY;

                if (board[head->x][head->y - 1] == BODY)
                    return 0;

                board[head->x][head->y - 1] = HEAD;

                move_body(head, direction);
            }
            break;
    }

    if (cherry_check)
    {
        add_ring(head);
        spawn_cherry(head, cherry);
        increase_score();
    }

    return alive;
}

void spawn_cherry(struct body *head, struct point *cherry)
{
    srand(time(NULL));

    int occupied = 0;
    int row = 0;
    int column = 0;

    do {
        occupied = 0;
        row = rand() % BOARD_SIZE;
        column = rand() % BOARD_SIZE;
        struct body *curr = head;
        while (curr->next)
        {
            if (curr->x == row && curr->y == column)
            {
                occupied = 1;
                break;
            }
            curr = curr->next;
        }
    } while(occupied);

    cherry->x = row;
    cherry->y = column;
}
