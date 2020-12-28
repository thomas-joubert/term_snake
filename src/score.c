#include "score.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int score;
int hiscore;

void increase_score(void)
{
    score++;
}

void print_score(void)
{
    printf("Score : %d        High score : %d\n", score, hiscore);
}

void load_hiscore(void)
{
    FILE *hiscore_file = fopen("score", "r");

    // The file is not yet created, thus the hiscore is set to 0
    if (!hiscore_file)
    {
        hiscore = 0;
        return;
    }

    char *line = NULL;
    size_t lim = 0;

    int ret = getline(&line, &lim, hiscore_file);

    // Yeah, there is no input verification, it might crash
    if (!ret)
        hiscore = 0;
    else
        hiscore = atoi(line);

    if (line)
        free(line);

    fclose(hiscore_file);
}

void write_hiscore(void)
{
    FILE *hiscore_file = fopen("score", "w+");
    if (!hiscore_file)
        err(4, "Error during hiscore file opening");

    if (score > hiscore)
        fprintf(hiscore_file, "%d\n", score);

    fclose(hiscore_file);
}
