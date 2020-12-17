#ifndef DEF_BOARD
#define DEF_BOARD

#include "keyboard.h"

enum board
{
    EMPTY = 0,
    CHERRY,
    PLAYER,
    HEAD
};

struct point
{
    int x;
    int y;
};

void draw_board(int board[14][27]);
void init_board(int board[14][27]);

int move(int board[14][27], char direction, struct point *head);

#endif // end of board.h
