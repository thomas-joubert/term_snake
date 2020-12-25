#ifndef DEF_BOARD
#define DEF_BOARD

#include "keyboard.h"

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

void draw_board(int board[14][27]);
void init_board(int board[14][27]);

void spawn_cherry(int board[14][27]);
void add_ring(int board[14][27], struct point tail);

int move(int board[14][27], char direction, struct point *head);
struct point move_body(int board[14][27], struct point head, char direction);

#endif // end of board.h
