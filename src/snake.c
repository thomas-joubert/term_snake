#include "snake.h"
#include "stdio.h"
#include "stdlib.h"

struct body *init_snake(int x, int y)
{
    struct body * head = calloc(1, sizeof(struct body));
    head->x = x;
    head->y = y;

    struct body * first_ring = calloc(1, sizeof(struct body));
    first_ring->x = head->x;
    first_ring->y = head->y + 1;

    head->next = first_ring;

    return head;
}

void free_snake(struct body *head)
{
    if (!head)
        return;

    free_snake(head->next);
    free(head);
}
