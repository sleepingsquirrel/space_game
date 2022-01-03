#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

_player *load_enemies(const char *filename, Card *allcards)
{
    //Opens the given file
    FILE *file = fopen(filename, "r");
    //Exits function if file does not exist
    if (file == NULL)
    {
        return 0;
    }
    //Initializes all remaining variables to NULL
    _player *start = NULL;
    _player *current;
    char buffer;
    char words[5][50];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j <  50; j++)
        {
            words[i][j] = '\0';
        }
    }
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
            current = malloc(sizeof(_player));
            current->name = malloc(strlen(words[0]) + 1);
            strcpy(current->name, &words[0][0]);
            current->name[strlen(words[0])] = '\0';
            //Loops through the integer variable in the current enemy, setting them in order
            current->maxHealth = atoi(words[1]);
            current->health = current->maxHealth;
            current->maxOxygen = atoi(words[2]);
            current->oxygen = current->maxOxygen;
            current->maxEnergy = atoi(words[3]);
            current->energy = current->maxEnergy;
            char cardnames[MAX_CARDS][20];
            int c,d;
            c = 0;
            d = 0;
            for (int a = 0; a < MAX_CARDS; a++)
            {
                for (int b = 0; b <  20; b++)
                {
                    cardnames[a][b] = '\0';
                }
            }
            for (int h = 0; h < strlen(words[4]); h++)
            {
                d++;
                if (words[4][h] == ',')
                {
                    cardnames[c][d - 1] = '\0';
                    d = 0;
                    c++;
                }
                else
                {
                    cardnames[c][d - 1] = words[4][h];
                }
            }
            Card *card;
            int z = 0;
            for (int h = 0; cardnames[h][0] != '\0'; h++, z++)
            {
                card = find_card(cardnames[h], allcards);
                if (card != NULL)
                {
                    current->deck[h] = card;
                }
                else
                {
                    z--;
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

void free_enemy(_player *p)
{
    //Recursively calls this function for every card in the list
    if (p == NULL)
    {
        return;
    }
    if (p->next != NULL)
    {
        free_enemy(p->next);
    }

    //Frees this card
    if (p->name != NULL)
    {
        free(p->name);
    }
    free(p);
}

void print_enemy(_player *current)
{
    printf("Name: %s\n", current->name);
    printf("Oxygen: %i/%i\n", current->oxygen, current->maxOxygen);
    printf("Energy: %i/%i\n", current->energy, current->maxEnergy);
    printf("Health: %i/%i\n", current->health, current->maxHealth);
    printf("Deck:\n");
    for (int i = 0; i < MAX_CARDS; i++)
    {
        if (current->deck[i] != NULL)
        {
            print_card(current->deck[i]);
        }
    }
    printf("\n");
}