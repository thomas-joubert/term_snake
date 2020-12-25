#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>

#include "keyboard.h"
#include "board.h"
#include "snake.h"

int main (void)
{
    fd_set fds;
    int board[BOARD_SIZE][BOARD_SIZE] = { 0 };
    struct body *head = init_snake(7, 13);
    struct point *cherry = calloc(1, sizeof(struct point));

    struct termios term_options;
    struct termios term_former;

    char direction = 0;
    // The snake is starting with its tail on the left
    char prev_direction = 'q';
    int alive = 1;

    struct timeval timeout;

    if (tcgetattr(1, &term_options) == -1)
    {
        perror("Fetching terminal data failed");
        return 2;
    }

    if (tcgetattr(1, &term_former) == -1)
    {
        perror("Fetching terminal data failed");
        return 2;
    }

    spawn_cherry(head, cherry);
    draw_board(board, head, cherry);

    term_options.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_options);

    puts("Welcome to my snake game!");

    fflush(stdout);

    while (1)
    {
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        int rdy = select(1, &fds, NULL, NULL, &timeout);

        if (rdy)
        {
            prev_direction = direction;
            int count = read(STDIN_FILENO, &direction, 1);
            if (count == -1)
            {
                perror("Error while reading on stdin");
                return 2;
            }
        }

        usleep(timeout.tv_usec);

        if (direction == UP || direction == DOWN || direction == LEFT || direction == RIGHT)
        {
            // prevent from going backward
            if ((direction == UP && prev_direction == DOWN)
                    || (direction == DOWN && prev_direction == UP)
                    || (direction == RIGHT && prev_direction == LEFT)
                    || (direction == LEFT && prev_direction == RIGHT))
                direction = prev_direction;

            alive = move(board, direction, head, cherry);
        }
        else if (direction == ESC)
        {
            puts("\nYou pressed ESC, thanks for playing !");
            tcsetattr(STDIN_FILENO, TCSANOW, &term_former);
            break;
        }

        if (alive == 0)
        {
            puts("You died, how sad...");
            break;
        }
        else
            draw_board(board, head, cherry);

    }

    free_snake(head);
    free(cherry);

    return 0;
}
