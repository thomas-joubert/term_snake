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

// TODO : Handle corner case
void add_ring(struct body *head)
{
    struct body *curr = head->next;

    int x_curr = curr->x;
    int y_curr = curr->y;

    int x_prev = head->x;
    int y_prev = head->y;

    for (;curr->next; curr = curr->next)
    {
       int tmp_x = x_curr;
       int tmp_y = y_curr;

       x_curr = curr->x;
       y_curr = curr->y;

       x_prev = tmp_x;
       y_prev = tmp_y;
    }

    int modif_x = x_curr == 16;
    int modif_y = 0;
    if (!modif_x)
        modif_y = y_curr == 16;

    struct body * new_ring = calloc(1, sizeof(struct body));
    new_ring->x = x_curr + (x_prev - x_prev) - modif_x;
    new_ring->y = y_curr + (y_prev - y_prev) - modif_y;

    curr->next = new_ring;
}
