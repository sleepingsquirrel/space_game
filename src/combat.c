#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "main.h"
#include "effects.h"

const char *name_of_var_for_print_f[] = {"Oxygen Cost", "Energy Cost", "Health Cost", "# of Targets", "Oxygen Damage", "Energy Damage", "Health Damage"};
int test_enemy_oxygen = 10;
int test_enemy_energy = 10;
int test_enemy_health = 10;

int main(void)
{
    srand(time(0));
    _player *test = malloc(sizeof(_player));
    //printf("2\n");
    Card *start = loadcards("data/testcards.txt");
    //printf("3\n");
    int i = 0;
    for (i = 0; i < MAX_CARDS; i++)
    {
        test->deck[i] = NULL;
    }
    i = 0;
    for (Card *current = start; current != NULL; current = current->next)
    {
        test->deck[i] = current;
        i++;
    }
    for (i = 0; i < 6; (&(test->health))[i] = 10, i++);
    //printf("4\n");
    printf("%s\n", fight(test) ? "win": "loss");
    free_card(start);
    free(test);
}

bool fight(_player *player)
{
    //Initializes draw pile and sets it equal to the player's deck, randomly ordered
    Card *draw_pile[MAX_CARDS];
    memset(&draw_pile[0], 0, sizeof(draw_pile));
    for (int i = 0; player->deck[i]; i++)
    {
        draw_pile[i] = player->deck[i];
    }
    shuffle(draw_pile);
    //Initializes hand as 3 nulls
    Card *hand[3];
    for (int i = 0; i < 3; i++)
    {
        hand[i] = NULL;
    }
    //Draws cards from draw pile to fill hand
    int drawn = 0;
    for (int i = 0; i < 3; i++)
    {
        if (!hand[i])
        {
            hand[i] = draw_pile[drawn];
            drawn++;
        }
    }
    //Runs round-to-round combat for as long as it continues
    bool still_going = true;
    int result = 0;
    while (still_going)
    {
        //Prints important info for the player's turn
        printf("PLAYER - Oxygen: %i/%i  Energy: %i/%i   Health: %i/%i\n", player->oxygen, player->maxOxygen, player->energy, player->maxEnergy, player->health, player->maxHealth);
        printf("ENEMY - Oxygen: %i/%i  Energy: %i/%i   Health: %i/%i\n", test_enemy_oxygen, 10, test_enemy_energy, 10, test_enemy_health, 10);
        printf("PLAYER HAND:\n%s\n%s\n%s\n", hand[0]->name, hand[1]->name, hand[2]->name);
        //Gets input from the player in the form of a string
        printf("Action>");
    	char input[15];
    	fgets(input, 15, stdin);
    	int q;
    	for (q = 0; input[q] != '\n'; input[q] = tolower(input[q]), q++);
    	input[q] = '\0';
    	//Analyzes player input as one of 4 commands
    	int num[] = {2, 5, 3, 2};
    	char *strings[] = {
    		"help", "h",
    		"quit", "q", "surrender", "stop", "s",
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
    	//Uses command given by player
    	switch (last_command)
    	{
    	    case 0: /*help*/
    	        //prints help info
    	        printf("List of commands:\nhelp - this\nstop - leaves game\ninfo - gives info on a card\nplay - plays a card\n");
    	        break;
    	    case 1: /*surrender*/
    	        //TODO
    	        break;
    	    case 2: /*info*/
    	        //asks player to input the name of one of their cards, printing that card's information
    	        printf("Which card would you like to know about?\n");
    	        fgets(input, 15, stdin);
    	        for (q = 0; input[q] != '\n'; q++);
    	        input[0] = toupper(input[0]);
    	        input[q] = '\0';
    	        for (int i = 0; i < 3; i++)
    	        {
    	            if (!strcmp(input, hand[i]->name))
    	            {
    	                print_card(hand[i]);
    	            }
    	        }
    	        break;
    	    case 3: /*play*/
    	        //asks player for the name of one of their cards
    	        printf("Which card would you like to play?\n");
    	        fgets(input, 15, stdin);
    	        for (q = 0; input[q] != '\n'; q++);
    	        input[0] = toupper(input[0]);
    	        input[q] = '\0';
    	        for (int i = 0; i < 3; i++)
    	        {
    	            if (!strcmp(input, hand[i]->name))
    	            {
    	                //plays the named card
    	                play_card(hand[i], player);
    	                //draws cards back up to hand limit, reshuffles when needed
    	                if(draw_pile[drawn] != NULL)
    	                {
    	                    shuffle(draw_pile);
    	                    drawn = 0;
    	                }
    	                for (int j = 0; j < 3; j++)
    	                {
    	                    if (hand[j] == draw_pile[drawn])
    	                    {
    	                        j = 0;
    	                        drawn++;
    	                    }
    	                }
    	                hand[i] = draw_pile[drawn];
                        drawn++;
                        break;
    	            }
    	        }
    	        break;
    	}
    	//
    	result = check_life(player);
    	still_going = result != 0;
    }
    return result < 0;
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

void play_card(Card *card, _player *player)
{
    player->oxygen = fmin(player->maxOxygen, fmax(0, player->oxygen + card->cost_o));
    player->energy = fmin(player->maxEnergy, fmax(0, player->energy + card->cost_e));
    player->health = fmin(player->maxHealth, fmax(0, player->health + card->cost_h));
    test_enemy_oxygen = fmin(10, fmax(0, test_enemy_oxygen - card->dam_o));
    test_enemy_energy = fmin(10, fmax(0, test_enemy_energy - card->dam_e));
    test_enemy_health = fmin(10, fmax(0, test_enemy_health - card->dam_h));
    if (card->effects[0] == true)
    {
        //TODO
    }
    if (card->effects[1] == true)
    {
        //TODO
    }

    enemy_turn(player);
}

void enemy_turn(_player *player)
{
   player->oxygen = fmin(player->maxOxygen, fmax(0, player->oxygen - 1));
   player->energy = fmin(player->maxEnergy, fmax(0, player->energy - 1));
   player->health = fmin(player->maxHealth, fmax(0, player->health - 1));
}

int check_life(_player *player)
{
    if (player->oxygen == 0)
    {
        return 1;
    }
    if (player->energy == 0)
    {
        return 1;
    }
    if (player->health == 0)
    {
        return 1;
    }
    if (test_enemy_oxygen == 0)
    {
        return -1;
    }
    if (test_enemy_energy == 0)
    {
        return -1;
    }
    if (test_enemy_health == 0)
    {
        return -1;
    }
    return 0;
}