#ifndef SNAKE_H
#define SNAKE_H

struct body
{
    int x;
    int y;
    struct body *next;
};

struct body *init_snake(int x, int y);
void free_snake(struct body *head);

#endif // end of snake.h
