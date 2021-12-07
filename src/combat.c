#include <stdio.h>
#include <stdbool.h>

#include "main.h"

bool fight(_player *player)
{

}

void shuffle(Card *deck)
{

    Card *c[MAX_CARDS] = malloc(sizeof(Card) * MAX_CARDS);

    for (Card *current = deck, int i = 0; current != NULL; current = current->next, i++)
    {
        c[i] = current;
    }

    for (int i = 0; c[i]; i++)
    {
        int j = i + rand() / (RAND_MAX / (MAX_CARDS - i) + 1);
        Card *temp = c[i];
        c[i] = c[j];
        c[j] = temp;
    }

    for (Card *current = deck, int i = 0; c[i]; current = current->next, i++)
    {
        current = c[i];
    }

    free(c);
}