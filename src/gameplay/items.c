#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include "../main.h"


void inventory(_player *player)
{
    bool has_items = false;
    printf("Your Items:\n\n");
    for (int i = 0; i < ITEMS; i++)
        if (player->items[i])
        {
            printf("%i. %s\n", i, player->items[i]->name);
            printf("\t%s\n", player->items[i]->type ? player->items[i]->type  - 1 ? "Potion" : "Wand" : "Chip");
            has_items = true;
        }
    if (!has_items)
    {
        printf("You have no items\n");
        return;
    }
    else
    {
        printf("Chips must be equiped at a medbay\n");
        printf("d: Drink Potion, e, Equip Wand\n>");
        char * c;
        char * d;
        fgets(c, 1, stdin);
        printf("Input number>");
        fgets(d, 1, stdin);
        switch(tolower(*c))
        {
            case 'd':
                if (player->items[*d - '0'])
                    if (player->items[*d - '0']->type == 3)
                    {
                        use_potion(player, player->items[*d - '0']);
                    }
            case 'e':
                if (player->items[*d - '0'])
                    if (player->items[*d - '0']->type == 2)
                    {
                        player->wand = player->items[*d - '0'];
                        printf("%s sucessfully equiped\n", player->items[*d - '0']->name);
                    }
        }
    }
}

item *gen_item(_player *player)
{
    Card *start = getCards();
    item *current = malloc(sizeof(item));
    current->spell = NULL;
    current->type = (rand() % 2);
    int x = rand() % 10;
    int level = fmax(fmin(-x * x / abs(x) + 1.25 * fmax(player->sat->level, 1), 0), 10);
    current->level = level;
    if (!current->type)
    {
       //chip
       memset(&current->o, 0, sizeof(int) * 4);
       int stat;
       int k;
       int change;
       int i;
       for (i = 0; i < level; i++);
       {
           stat = rand() % 4;
           k = rand() % 4;
           change = (k != 1) ? rand() % level - i : 0 - rand() % level - i;
           switch (stat)
           {
               case 0:
                    current->o += change;
                    break;
                case 1:
                    current->h += change;
                    break;
                case 2:
                    current->e += change;
                    break;
                case 3:
                    current->s += change;
                    break;
           }
           i += change;
       }

    }
    else
    {
        //other items
        current->type += rand() % 2;
        if (current->type == 2)
        {
            int spell_level =fmin(fmax(current->level / 2.5 - 1  + floor((float)(rand() % 10) / 10) + floor((float)(rand() % 10) / 10) + floor((float)(rand() % 10) / 10), 0), 3);
            int num = 0;
            printf("%i\n", spell_level);
            //wand
            for (Card *card = start; card; card = card->next)
            {
                if(card->rarity == spell_level)
                {
                    num++;
                }
            }
            Card *leveld_cards[num];
            num = 0;
            for (Card *card = start; card; card = card->next)
            {
                if(card->rarity == spell_level)
                {
                    leveld_cards[num] = card;
                    num++;
                }
            }
            current->spell = leveld_cards[rand() % sizeof(leveld_cards) / sizeof(Card*)];
            memset(&current->o, 0, sizeof(int) * 4);
            int diff = current->spell->rarity - fmax(current->level / 2.5 - 1, 0) * 5;
            while (diff > 0)
            {
                (&current->o)[rand() % 4] -= 1;
                diff -= 1;
            }
        }
        else
        {
            //potion
            for (int i = 0; i < ITEM_EFFECT_COUNT; i++)
            {
                current->effect[i] = 0;
            }
            for (int i = 0; i < level / 2.5; i++)
            {
                current->effect[rand() % level] = 1;
            }
        }
    }
    current->name = gen_item_name(current);
    return current;
}

void use_potion(_player *player, item *current)
{
    int seen_rooms;
    if (current->type = 3)
    {
        for (int i = ITEM_EFFECT_COUNT; i < 0; i--)
        {
            if (current->effect[i])
            {
                switch (i)
                {
                    case 0:
                        player->health += rand() & 10 - 3;
                        break;
                    case 1:
                        (&player->health)[rand() % 3] += rand() % 10 - 3;
                        break;
                    case 2:
                        player->maxHealth += rand() % 3 - 1;
                        break;
                    case 3:
                        (&player->maxHealth)[rand() % 3] += rand() % 3 - 1;
                        break;
                    case 4:
                        player->gold += rand() % 70 - 10;
                        break;
                    case 5:
                        player->health = player->maxHealth / (rand() % 2 + 1);
                        break;
                    case 6:
                        encounter(player);
                        break;
                    case 7:
                        player->room = player->sat->starting_room;
                        break;
                    case 8:
                        gen_loot(player);
                        break;
                    case 9:
                        seen_rooms = 0;
                        for (room *current = player->sat->rooms; current; current = current->next)
                            if (current->seen)
                            {
                                seen_rooms++;
                            }
                        if (seen_rooms == 0)
                        {
                            break;
                        }
                        int rand_room = rand() % seen_rooms;
                        seen_rooms = 0;
                        for (room *current = player->sat->rooms; current; current = current->next)
                        {
                            if (rand_room == seen_rooms)
                            {
                                player->room = current;
                                break;
                            }
                            if (current->seen)
                            {
                                seen_rooms++;
                            }
                        }
                        break;
                }
            }
        }
    }
}

char *gen_item_name(item *input)
{
    char *word1_options[] = {"Cursed\0", "Broken\0", "Jinxed\0", "Painful\0"};
    char *word1 = NULL;
    char *word2_options[] = {"Super \0", "Greater \0", "Extreme \0", "Hyper-\0", "Minor \0", "Slight \0", "Marginal \0", "Mini-\0"};
    char *word2 = NULL;
    char *word3_options[] = {"Balance\0", "Unity\0", "Combined\0",
                                "Lung\0", "Gills\0", "Air\0",
                                "Power\0", "Stamina\0", "Generator\0",
                                "Vitality\0", "Endurance\0", "Life\0",
                                "Swiftness\0", "Agiliy\0", "Momentum\0"
                            };
    char *word3 = NULL;
    char *word4 = NULL;
    char *word4_options[] = {"Staff\0", "Wand\0", "Orb\0", "Blaster\0", "Cannone\0", "Ray\0"};
    int random;
    if (!input->type)
    {
        if (input->o <= 0 - input->level || input->e <= 0 - input->level || input->h <= 0 - input->level || input->s <= 0 - input->level)
        {
            random = rand() % 4;
            word1 = malloc(strlen(word1_options[random]) + 1);
            word1 = word1_options[random];
        }
        if (input->level > 7)
        {
            random = rand() % 4;
            word2 = malloc(strlen(word2_options[random]) + 1);
            word2 = word2_options[random];
        }
        else if (input->level < 4)
        {
            random = (rand() % 4) + 4;
            word2 = malloc(strlen(word2_options[random]) + 1);
            word2 = word2_options[random];
        }
        int best = -10000000;
        int match = 0;
        for (int i = 0; i < 4; i++)
        {
            if ((&(input->o))[i] > best)
            {
                best = (&(input->h))[i];
                match = 0;
            }
            else if ((&(input->o))[i] == best)
            {
                match++;
            }
        }
        if (match > 0)
        {
            random = rand() % 3;
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                if ((&(input->o))[i] == best)
                {
                    random = rand() % 3 + 3 * (i + 2);
                    word3 = malloc(strlen(word3_options[random]) + 1);
                    word3 = word3_options[random];
                }
            }
        }
        word4 = malloc(sizeof(char) * 9);
        word4 = "Chip 3000 \0";
    }
    else if (input->type == 2)
    {
        if (input->spell->rarity > fmax(input->level / 2.5 - 1, 0))
        {
            random = rand() % 4;
            word1 = malloc(strlen(word1_options[random]) + 1);
            word1 = word1_options[random];
        }
        word2 = malloc(strlen(input->spell->name));
        for (int i = 0; i < strlen(input->spell->name); word2[i] = input->spell->name[i], i++);
        random = rand() % 6;
        word4 = malloc(strlen(word4_options[random]) + 1);
        word4 = word4_options[random];
    }
    else
    {
        return "Mysterious Potion\0";
    }
    char *name = NULL;
    int size = 3;
    if (word1 != NULL)
        size += strlen(word1);
    if (word2 != NULL)
        size += strlen(word2);
    if (word3 != NULL)
        size += strlen(word3);
    if (word4 != NULL)
        size += strlen(word4);
    name = malloc(size);
    int c = 0;
    if (word1 != NULL)
    {
        for (int i = 0; i < strlen(word1); name[c] = word1[i], c++, i++);
        name[c] = ' ';
        c++;
        free(word1);
    }
    if (word2 != NULL)
    {
        for (int i = 0; i < strlen(word2); name[c] = word2[i], c++, i++);
        free(word2);
    }
    if (word3 != NULL)
    {
        for (int i = 0; i < strlen(word3); name[c] = word3[i], c++, i++);
        free(word3);
    }
    name[c] = ' ';
    c++;
    if (word4 != NULL)
    {
        for (int i = 0; i < strlen(word4); name[c] = word4[i], c++, i++);
        free(word4);
    }
    name[c] = '\0';
    return name;
}

void gen_loot(_player *player)
{
    printf("-----------------------------------------------------\n");
    double x = rand() % 20 - 10;
    int gold = (pow(x, 2) / 25 + 10) * player->sat->level;
    player->gold += gold;
    printf("You gain %i gold.\n", gold);
    /*int q;
    for (q = 0; player->items[q]; q++);
    if (q == ITEMS)
    {
        printf("You cannot hold any more items.\n");
    }
    else
    {
        player->items[q] = gen_item(player);
        printf("You gain %s.\n", player->items[q]->name);
    }*/
    printf("-----------------------------------------------------\n");
}

void free_item(item *current)
{
    free(current->name);
    free(current);
}