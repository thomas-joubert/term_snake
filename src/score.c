#include "score.h"
#include "stdio.h"

int score;

void increase_score(void)
{
    score++;
}

void print_score(void)
{
    printf("Score : %d\n", score);
}
