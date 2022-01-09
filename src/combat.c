#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "main.h"

const char *name_of_var_for_print_f[] = {"Oxygen Cost", "Energy Cost", "Health Cost", "# of Targets", "Oxygen Damage", "Energy Damage", "Health Damage"};

//Status effects
bool player_thorns = false;
bool player_bleeding = false;
bool player_targeting = false;
bool enemy_thorns = false;
bool enemy_bleeding = false;
bool enemy_targeting = false;

int main(void)
{
    srand(time(0));
    _player *test = malloc(sizeof(_player));
    test->next = NULL;
    //printf("2\n");
    Card *start = loadcards("data/cards.txt");
    _player *enemy_start = load_enemies("data/enemies.txt", start);
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
        if (i == MAX_CARDS)
        {
            break;
        }
    }
    for (i = 0; i < 6; (&(test->health))[i] = 10, i++);
    printf("%s\n", fight(test, enemy_start) ? "win": "loss");
    free_enemy(enemy_start);
    free_card(start);
    free(test);
}

bool fight(_player *player, _player *enemy)
{
    //Initializes draw pile and sets it equal to the player's deck, randomly ordered
    Card *draw_pile[MAX_CARDS];
    memset(&draw_pile[0], 0, sizeof(draw_pile));
    for (int i = 0; player->deck[i] != NULL; i++)
    {
        draw_pile[i] = player->deck[i];
    }
    shuffle(draw_pile);
    //Sets enemy draw pile
    Card *enemy_cards[MAX_CARDS];
    memset(&enemy_cards[0], 0, sizeof(enemy_cards));
    for (int i = 0; enemy->deck[i]; i++)
    {
        enemy_cards[i] = enemy->deck[i];
    }
    shuffle(enemy_cards);
    int enemy_card_num = 0;
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
        printf("%s - Oxygen: %i/%i  Energy: %i/%i   Health: %i/%i\n", enemy->name, enemy->oxygen, enemy->maxOxygen, enemy->energy, enemy->maxEnergy, enemy->health, enemy->maxHealth);
        printf("PLAYER HAND:\n%s\n%s\n%s\n", hand[0]->name, hand[1]->name, hand[2]->name);
        //Gets input from the player in the form of a string
        printf("Action>");
    	char input[15];
    	memset(&input[0], 0, sizeof(input));
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
    	        return false;
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
    	        for (q = 0; input[q] != '\n'; q++)
    	        input[0] = toupper(input[0]);
    	        input[q] = '\0';
    	        for (int i = 0; i < 3; i++)
    	        {
    	            if (!strcmp(input, hand[i]->name))
    	            {
    	                //plays the named card
    	                play_card(hand[i], enemy_cards[enemy_card_num], player, enemy);
    	                //draws cards back up to hand limit, reshuffles when needed
    	                enemy_card_num++;
    	                if (enemy_cards[enemy_card_num] == NULL)
    	                {
    	                    shuffle(enemy_cards);
    	                    enemy_card_num = 0;
    	                }
    	                if (draw_pile[drawn] != NULL)
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
    	result = check_life(player, enemy);
    	still_going = result == 0;
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
    printf("name: %s\n", current->name);
    for (int i = 0; i < 7; i++)
    {
        printf("%s:", name_of_var_for_print_f[i]);
        printf("%i\n", (&(current->cost_o))[i]);
    }
    for (int i = 0; i < EFFECT_COUNT; i++)
    {
        printf("effects %i: %i\n", i,  current->effects[i] ? 1 : 0);
    }
    printf("\n");
}

void play_card(Card *card, Card *enemy_card, _player *player, _player *enemy)
{
    int modifier = 1;
    if (enemy_card->effects[0] == true)
    {
        modifier *= 2;
    }
    if (enemy_card->effects[1] == true)
    {
        modifier *= 0;
    }
    if (player_targeting)
    {
        modifier *= 2;
    }
    enemy_thorns = enemy_card->effects[2];
    player_bleeding = enemy_card->effects[3];
    enemy_targeting = enemy_card->effects[5];
    player->oxygen = fmin(player->maxOxygen, fmax(0, player->oxygen - card->cost_o));
    player->energy = fmin(player->maxEnergy, fmax(0, player->energy - card->cost_e));
    player->health = fmin(player->maxHealth, fmax(0, player->health - card->cost_h));
    enemy->oxygen = fmin(enemy->maxOxygen, fmax(0, enemy->oxygen - card->dam_o * modifier));
    enemy->energy = fmin(enemy->maxEnergy, fmax(0, enemy->energy - card->dam_e * modifier));
    enemy->health = fmin(enemy->maxHealth, fmax(0, enemy->health - card->dam_h * modifier));
    if (player_bleeding)
    {
        player->health = fmin(player->maxHealth, fmax(0, player->health - 1));
    }
    if (enemy_thorns && card->dam_h >= 0)
    {
        player->health = fmin(player->maxHealth, fmax(0, player->health - 1));
    }
    modifier = 1;
    if (card->effects[0] == true)
    {
        modifier *= 2;
    }
    if (card->effects[1] == true)
    {
        modifier *= 0;
    }
    if (enemy_targeting)
    {
        modifier *= 2;
    }
    player_thorns = card->effects[2];
    enemy_bleeding = card->effects[3];
    player_targeting = card->effects[5];
    if (card->effects[4] == true)
    {
        enemy->health = fmin(10, fmax(0, enemy->health - 1));
    }
    enemy_turn(enemy_card, player, enemy, modifier);
}

void enemy_turn(Card *enemy_card, _player *player, _player *enemy, int modifier)
{
    enemy->oxygen = fmin(enemy->maxOxygen, fmax(0, enemy->oxygen - enemy_card->cost_o));
    enemy->energy = fmin(enemy->maxEnergy, fmax(0, enemy->energy - enemy_card->cost_e));
    enemy->health = fmin(enemy->maxHealth, fmax(0, enemy->health - enemy_card->cost_h));
    player->oxygen = fmin(player->maxOxygen, fmax(0, player->oxygen - enemy_card->dam_o * modifier));
    player->energy = fmin(player->maxEnergy, fmax(0, player->energy - enemy_card->dam_e * modifier));
    player->health = fmin(player->maxHealth, fmax(0, player->health - enemy_card->dam_h * modifier));
    if (enemy_bleeding)
    {
        enemy->health = fmin(enemy->maxHealth, fmax(0, enemy->health - 1));
    }
    if (player_thorns && enemy_card->dam_h >= 0)
    {
        enemy->health = fmin(enemy->maxHealth, fmax(0, enemy->health - 1));
    }
    if (enemy_card->effects[4] == true)
    {
        player->health = fmin(10, fmax(0, player->health - 1));
    }
    printf("Enemy played: %s\n", enemy_card->name);
    printf("-----------------------------------------------------\n");
}

int check_life(_player *player, _player *enemy)
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
    if (enemy->oxygen == 0)
    {
        return -1;
    }
    if (enemy->energy == 0)
    {
        return -1;
    }
    if (enemy->health == 0)
    {
        return -1;
    }
    return 0;
}