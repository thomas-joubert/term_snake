#include "score.h"
#include "stdio.h"

int score;

void increase_score(void)
{
    score++;
    printf("Score : %d\n", score);
}
