#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "main.h"

const char *name_of_var_for_print_f[] = {"cost_o", "cost_e", "cost_h", "targets", "dam_o", "dam_e", "dam_h"};

int main(void)
{
    //printf("1\n");
    srand(time(0));
    _player *test = malloc(sizeof(_player));
    //printf("2\n");
    Card *start = loadcards("data/testcards.txt");
    //printf("3\n");
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
    //printf("4\n");
    fight(test);
    free_card(start);
    free(test);
}

bool fight(_player *player)
{
    Card *draw_pile[MAX_CARDS];
    memset(&draw_pile[0], 0, sizeof(draw_pile));
    for (int i = 0; player->deck[i]; i++)
    {
        draw_pile[i] = player->deck[i];
    }
    Card *discard_pile[MAX_CARDS];
    //printf("5\n");
    shuffle(draw_pile);
    //printf("6\n");
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
            //print_card(hand[i]);
        }
        //printf("7\n");
    }
    bool still_going = true;
    while (still_going)
    {
        printf("PLAYER - Oxygen: %i/%i  Energy: %i/%i   Health: %i/%i\n", player->oxygen, player->maxOxygen, player->energy, player->maxEnergy, player->health, player->maxHealth);
        printf("ENEMY - Oxygen: %i/%i  Energy: %i/%i   Health: %i/%i\n", 10, 10, 10, 10, 10, 10);
        printf("PLAYER HAND:\n%s\n%s\n%s\n", hand[0]->name, hand[1]->name, hand[2]->name);
        printf("Action>");
        //memset(&input[0], 0, sizeof(draw_pile));
    	char input[15];
    	fgets(input, 15, stdin);
    	int q;
    	for (q = 0; input[q] != '\n'; input[q] = tolower(input[q]), q++);
    	//end the string
    	input[q] = '\0';
    	int num[] = {2, 4, 3, 2};
    	char *strings[] = {
    		"help", "h",
    		"quit", "q", "close", "stop",
    		"info", "card", "i",
    		"play", "p",
    	};
    	int numpos = 0;
    	q = 0;
    	int last_command;
    	for (int curr = 0; q < sizeof(strings) / sizeof(char*); q++, curr++)
    	{
    	    if (curr == num[numpos])
    		{
    			numpos++;
    			curr = 0;
    		}
    		if (!strcmp(input, strings[q]))
    		{
    			last_command = numpos;
    			break;
    		}
    	}
    	switch (last_command)
    	{
    	    case 0:
    	        printf("List of commands:\nhelp - this\nstop - leaves game\ninfo - gives info on a card\nplay - plays a card\n");
    	        break;
    	    case 1:
    	        quit();
    	        still_going = false;
    	        break;
    	    case 2:
    	        printf("Which card would you like to know about?\n");
    	        fgets(input, 15, stdin);
    	        for (int i = 0; i < 3; i++)
    	        {
    	            if (strcmp(input, hand[i]->name))
    	            {
    	                printf("true");
    	                print_card(hand[i]);
    	            }
    	        }
    	        break;
    	    case 3:
    	        printf("Which card would you like to play?\n");
    	        fgets(input, 15, stdin);
    	        for (int i = 0; i < 3; i++)
    	        {
    	            if (!strcmp(input, hand[i]->name) == 0)
    	            {
    	                play_card(hand[i]);
    	            }
    	        }
    	        break;
    	}
    }
    return false;
}

void shuffle(Card *c[MAX_CARDS])
{
    int size = 0;
    for (int i = 0; i < MAX_CARDS; i++)
    {
        if (c[i] != NULL)
        {
            size++;
        }
    }
    for (int i = 0; i < size - 1; i++)
    {
        //printf("6.1, %i\n", i);
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        //printf("6.2, %i\n", i);
        Card *temp = c[i];
        c[i] = c[j];
        c[j] = temp;
        //printf("6.3\n");
    }
}

void print_card(Card *current)
{
    //printf("7.1\n");
    printf("name: %s\n", current->name);
    //printf("7.2\n");
    for (int i = 0; i < 7; i++)
    {
        printf("%s:", name_of_var_for_print_f[i]);
        printf("%i\n", (&(current->cost_o))[i]);
    }
    //printf("7.3\n");
    for (int i = 0; i < EFFECT_COUNT; i++)
    {
        printf("effects %i: %i\n", i,  current->effects[i] ? 1 : 0);
    }
    //printf("7.4\n");
    printf("\n");
}

void quit()
{
    printf("\n.\n");
}

void play_card(Card *card)
{
    printf("g\n");
}