#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"

const char *name_of_var_for_print_f[] = {"cost_o", "cost_e", "cost_h", "targets", "dam_o", "dam_e", "dam_h"};

int main(void)
{
    printf("1\n");
    _player *test = malloc(sizeof(_player));
    printf("2\n");
    Card *start = loadcards("data/testcards.txt");
    printf("3\n");
    int i = 0;
    for (int i = 0; i < MAX_CARDS; i++)
    {
        test->deck[i] = NULL;
    }
    for (Card *current = start; current != NULL; current = current->next)
    {
        test->deck[i] = current;
        i++;
    }
    printf("4\n");
    fight(test);
    free_card(start);
    free(test);
}

bool fight(_player *player)
{
    Card *draw_pile[MAX_CARDS];
    for (int i = 0; player->deck[i]; i++)
    {
        draw_pile[i] = player->deck[i];
    }
    Card *discard_pile[MAX_CARDS];
    printf("5\n");
    shuffle(draw_pile);
    printf("6\n");
    Card *hand[3];
    for (int i = 0; i < 3; i++)
    {
        hand[i] = NULL;
    }
    int drawn = 0;
    for (int i = 0; i < 3; i++)
    {
        if (!hand[i])
        {
            hand[i] = draw_pile[drawn];
            drawn++;
            print_card(hand[i]);
        }
        printf("7\n");
    }
    return false;
}

void shuffle(Card *c[MAX_CARDS])
{
    int size = 0;
    for (int i = 0; i < MAX_CARDS; i++)
    {
        size++;
    }
    for (int i = 0; i < size - 1; i++)
    {
        printf("6.1, %i\n", i);
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        printf("6.2, %i\n", i);
        Card *temp = c[i];
        c[i] = c[j];
        c[j] = temp;
        printf("6.3\n");
    }
}

void print_card(Card *current)
{
    printf("7.1\n");
    printf("name: %s\n", current->name);
    printf("7.2\n");
    for (int i = 0; i < 7; i++)
    {
        printf("%s:", name_of_var_for_print_f[i]);
        printf("%i\n", (&(current->cost_o))[i]);
    }
    printf("7.3\n");
    for (int i = 0; i < EFFECT_COUNT; i++)
    {
        printf("effects %i: %i\n", i,  current->effects[i] ? 1 : 0);
    }
    printf("7.4\n");
    printf("\n");
}