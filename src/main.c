#include <stdio.h>
#include <unistd.h>
#ifndef WIN
#include <termios.h>
#endif
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>

#include "keyboard.h"
#include "board.h"
#include "snake.h"
#include "score.h"

char get_input(fd_set *fds, char *direction)
{
    char prev_direction = 0;

    FD_ZERO(fds);
    FD_SET(0, fds);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 400000;

    load_hiscore();

    int rdy = select(1, fds, NULL, NULL, &timeout);

    if (rdy)
    {
        prev_direction = *direction;
        int count = read(STDIN_FILENO, direction, 1);
        if (count == -1)
        {
            perror("Error while reading on stdin");
            return 2;
        }
    }

    usleep(timeout.tv_usec);

    return prev_direction;
}

int main (void)
{
    fd_set fds;
    int board[BOARD_SIZE][BOARD_SIZE] = { 0 };

    struct body *head = init_snake(7, 13);
    struct point *cherry = calloc(1, sizeof(struct point));

    char direction = 0;
    // The snake is starting with its tail on the left
    char prev_direction = 'q';
    int alive = 1;

#ifndef WIN
    struct termios term_options;
    struct termios term_former;

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

    term_options.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_options);
#endif

    load_hiscore();

    spawn_cherry(head, cherry);
    draw_board(board, head, cherry);

    puts("Welcome to my snake game!");

    fflush(stdout);

    while (1)
    {
        prev_direction = get_input(&fds, &direction);

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
#ifndef WIN
            tcsetattr(STDIN_FILENO, TCSANOW, &term_former);
#endif
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

    write_hiscore();

    return 0;
}
