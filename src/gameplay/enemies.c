#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../main.h"

static int enemy_pref_c[] = {3, 0, 0, 5, 4, 0, 2, 2, 1, 2};
static int enemy_pref_usa[] = {1, 2, 5, 4, 0, 5, 0, 2, 1, 3};
static int enemy_pref_un[] = {1, 3, 0, 4, 5, 4, 3, 3, 0, 2};
static int enemy_pref_a[] = {0, 2, 5, 3, 4, 0, 0, 0, 0, 1};
static int enemy_pref_s[] = {1, 2, 5, 0, 5, 4, 3, 3, 2, 0};

_player *load_enemies(const char *filename)
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
            memset(&current->deck[0], 0, sizeof(current->deck));
            current->name = malloc(strlen(words[0]) + 1);
            strcpy(current->name, &words[0][0]);
            current->name[strlen(words[0])] = '\0';
            //Loops through the integer variable in the current enemy, setting them in order
            current->maxHealth = atoi(words[3]);
            current->health = current->maxHealth;
            current->maxOxygen = atoi(words[1]);
            current->oxygen = current->maxOxygen;
            current->maxEnergy = atoi(words[2]);
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
            for (int h = 0; cardnames[h][0] != '\0'; h++)
            {
                card = find_card(cardnames[h]);
                if (card != NULL)
                {
                    current->deck[h] = card;
                }
                else
                {
                    current->deck[h] = NULL;
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

bool encounter(_player *player)
{
    int x = player->sat->level;
    printf("%i", x);
    int random;
    int sum = 0;
    for (int i = 0; i < x; sum += rand() % 2, i++);
    _player *enemy_start = getEnemies();
    int tier = fmin(fmax(sum, 1), 5);
    int preferences[10];
    switch (player->sat->factionID)
    {
        case 0:
            for (int i = 0; i < 10; preferences[10 - i] = enemy_pref_usa[i], i++);
            break;
        case 1:
            for (int i = 0; i < 10; preferences[10 - i] = enemy_pref_un[i], i++);
            break;
        case 2:
            for (int i = 0; i < 10; preferences[10 - i] = enemy_pref_c[i], i++);
            break;
        case 3:
            for (int i = 0; i < 10; preferences[10 - i] = enemy_pref_s[i], i++);
            break;
        case 4:
            for (int i = 0; i < 10; preferences[10 - i] = enemy_pref_a[i], i++);
            break;
    }
    int q = 0;
    int p = 0;
    _player *enemy_table[4];
    memset(&enemy_table[0], 0, sizeof(enemy_table));
    for (_player *current = enemy_start; current != NULL; q++, current = current->next)
    {
        if (preferences[q] == tier)
        {
            enemy_table[p] = current;
            p++;
        }
    }
    for (p = 0; enemy_table[p] != NULL; p++);
    random = rand() % p;
    printf("You enounter a hostile %s.\n", enemy_table[random]->name);
    bool result = fight(player, enemy_table[random]);
    printf("-----------------------------------------------------\n");
    if (result)
    {
        printf("Congratulations, you won!\n");
        gen_loot(player);
    }
    else
    {
        printf("Uh-oh, you lost!\n");
    }
    return result;
}