#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


//https://www.geeksforgeeks.org/function-pointer-in-c/

#define EFFECT_COUNT 2
#define MAX_CARDS 6

typedef struct _Card {
    //Card name
    char *name;
    //Cost in oxygen, energy, health
    int cost_o;
    int cost_e;
    int cost_h;
    //amount of targets
    int targets;
    //damage in oxygen, energy, health
    int dam_o;
    int dam_e;
    int dam_h;
    //list of which effects this card does and does not have
    bool effects[EFFECT_COUNT];
    struct _Card *next;
} Card;

Card *loadcards(const char *filename);
void free_card(Card *next);

int main(void)
{
    Card *start = loadcards("data/testcards.txt");
    for (Card *current = start; current != NULL; current = current->next)
    {
        printf("name:%s\n", current->name);
        printf("dam:%i\n", current->dam_h);
    }
    free_card(start);
}

void free_card(Card *next)
{
    if (next->next != NULL)
    {
        free_card(next->next);
    }
    free(next->name);
    free(next);
}

Card *loadcards(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }
    Card *start = NULL;
    char *buffer = malloc(50);
    char words[9][sizeof(buffer)];
    for (int i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = '\0';
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < (int) sizeof(buffer); j++)
        {
            words[i][j] = '\0';
        }
    }
    while (fgets(buffer, sizeof(buffer), file))
    {   
        Card *current = malloc(sizeof(Card));
        int j = 0;
        int k = 0;
        for (int i = 0; buffer[i]; i++)
        {
            k++;
            if (buffer[i] == ';')
            {
                words[j][k - 1] = '\0';
                k = 0;  
                j++;
            }
            else
            {
                words[j][k - 1] = buffer[i];
            }
        }
        current->name = malloc(sizeof(words[0]) + 1);
        strcpy(current->name, &words[0][0]);
        current->name[(int) sizeof(words[0])] = '\0';
        for (int h = 0; h < 7; h++)
        {
            for (int i = 0; words[h][i]; i++)
            {
                if (isdigit(words[h][i]))
                {
                    (&(current->cost_o))[h] = atoi(words[h]);
                }
            }
        }
        // cost_o   
        // cost_e
        // current->cost_h
        // current->targets = atoi(words[4]);
        // current->dam_o = atoi(words[5]);
        // current->dam_e = atoi(words[6]);
        // current->dam_h = atoi(words[7]);
        for (int i = 0; i < EFFECT_COUNT; i++)
        {
            if (words[8][i] == '1')
            {
                current->effects[i] = true;
            }
            else
            {
                current->effects[i] = false;
            }
        }
        current->next = start;
        start = current;
    }

    free(buffer);
    fclose(file);
    return start;
}




