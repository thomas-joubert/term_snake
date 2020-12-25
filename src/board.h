#ifndef DEF_BOARD
#define DEF_BOARD

#include "keyboard.h"
#include "snake.h"

#define BOARD_SIZE 17

enum board
{
    EMPTY = 0,
    CHERRY = 1,
    BODY = 2,
    HEAD = 3
};

struct point
{
    int x;
    int y;
};

void draw_board(int board[BOARD_SIZE][BOARD_SIZE], struct body *head, struct point *cherry);
void init_board(int board[BOARD_SIZE][BOARD_SIZE]);

void spawn_cherry(struct body *head, struct point *cherry);
void add_ring(struct body *head);

int move(int board[BOARD_SIZE][BOARD_SIZE], char direction, struct body *head, struct point *cherry);
void move_body(struct body *head, char direction);

#endif // end of board.h
