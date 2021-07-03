#include <stdio.h>
#include <unistd.h>
#ifndef WIN
#include <termios.h>
#endif
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#include "keyboard.h"
#include "board.h"
#include "snake.h"
#include "score.h"

struct keyboard keyboard =
    {.UP = 'w', .DOWN = 's', .LEFT = 'a', .RIGHT = 'd', .ESC = 27 };

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

int set_termios(struct termios* options, struct termios *former)
{
    if (tcgetattr(1, options) == -1)
    {
        perror("Fetching terminal data failed");
        return 0;
    }

    if (tcgetattr(1, former) == -1)
    {
        perror("Fetching terminal data failed");
        return 0;
    }

    options->c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, options);

    return 1;
}

int main (int argc, char *argv[])
{
    fd_set fds;
    int board[BOARD_SIZE][BOARD_SIZE] = { 0 };

    if (argc == 2)
        keyboard = set_layout(argv[1]);

    struct body *head = init_snake(7, 13);
    struct point *cherry = calloc(1, sizeof(struct point));

    char dir = 0;
    // The snake is starting with its tail on the left
    char prev_dir = 'q';
    int alive = 1;

    load_hiscore();

    spawn_cherry(head, cherry);
    draw_board(board, head, cherry);

    puts("Welcome to my snake game!");

    fflush(stdout);

    while (1)
    {
        prev_dir = get_input(&fds, &dir);

        if (dir == keyboard.UP || dir == keyboard.DOWN
                || dir == keyboard.LEFT || dir == keyboard.RIGHT)
        {
            // prevent from going backward
            if ((dir == keyboard.UP && prev_dir == keyboard.DOWN)
                    || (dir == keyboard.DOWN && prev_dir == keyboard.UP)
                    || (dir == keyboard.RIGHT && prev_dir == keyboard.LEFT)
                    || (dir == keyboard.LEFT && prev_dir == keyboard.RIGHT))
                dir = prev_dir;

            alive = move(board, dir, head, cherry);
        }
        else if (dir == keyboard.ESC)
        {
            puts("\nYou pressed ESC, thanks for playing !");
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
