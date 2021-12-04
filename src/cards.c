#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

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

const char *name_of_var_for_print_f[] = {"cost_o", "cost_e", "cost_h", "targets", "dam_o", "dam_e", "dam_h"};
Card *loadcards(const char *filename);
void free_card(Card *next);

int main(void)
{
    Card *start = loadcards("data/testcards.txt");
    for (Card *current = start; current != NULL; current = current->next)
    {
        printf("name: %s\n", current->name);
        for (int i = 0; i < 7; i++)
            printf("%s: %i\n", name_of_var_for_print_f[i], (&(current->cost_o))[i]);
        for (int i = 0; i < EFFECT_COUNT; i++)
        {
            printf("effects %i: %i\n", i,  current->effects[i] ? 1 : 0);
        }
        printf("this %p -> next %p\n", current, current->next);
        printf("\n");

    }
    free_card(start);
}

void free_card(Card *p)
{
    //Recursively calls this function for every card in the list
    if (p == NULL)
    {
        return;
    }
    if (p->next != NULL)
    {
        free_card(p->next);
    }

    //Frees this card
    if (p->name != NULL)
    {
        free(p->name);
    }
    free(p);
}

Card *loadcards(const char *filename)
{
    //Opens the given file
    FILE *file = fopen(filename, "r");
    //Exits function if file does not exist
    if (file == NULL)
    {
        return 0;
    }
    //Initializes all remaining variables to NULL
    Card *start = NULL;
    Card *current;
    char buffer;
    char words[9][50];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j <  50; j++)
        {
            words[i][j] = '\0';
        }
    }
    //TODO: switch to fread
    int j,k;
    j = 0;
    k = 0;
    while (fread(&buffer, sizeof(char), 1, file))
    {
        if (buffer != '\n')
        {
            k++;
            if (buffer == ';')
            {
                words[j][k - 1] = '\0';
                k = 0;
                j++;
            }
            else
            {
                words[j][k - 1] = buffer;
            }
        }
        else
        {
            current = malloc(sizeof(Card));
            current->name = malloc(strlen(words[0]) + 1);
            strcpy(current->name, &words[0][0]);
            current->name[strlen(words[0])] = '\0';
            //Loops through the integer variable in the current carding, setting them in order
            for (int h = 0; h < 7; h++)
            {
                for (int i = 0; words[h][i]; i++)
                {
                    if (isdigit(words[h][i]))
                    {
                        //Sets the next variable in the card to the next word.
                        (&(current->cost_o))[h]/*the h'th variable after the cost_o of current*/ = atoi(words[h]);
                    }
                }
            }
            //Scrolls through marking whether they are true or false for this card
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
            //Places current at the front of the linked list
            current->next = start;
            start = current;
            j = 0;
        }
    }
    //Frees buffer and file
    fclose(file);
    //Returns the start of the linked list.
    return start;
}




