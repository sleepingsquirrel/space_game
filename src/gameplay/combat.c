#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "../main.h"

const char *name_of_var_for_print_f[] = {"Oxygen Cost", "Energy Cost", "Health Cost", "Rarity", "Oxygen Damage", "Energy Damage", "Health Damage"};

//Status effects
bool player_thorns = false;
bool player_bleeding = false;
bool player_targeting = false;
bool enemy_thorns = false;
bool enemy_bleeding = false;
bool enemy_targeting = false;

/*int main(void)
{
    srand(time(0));
    _player *test = malloc(sizeof(_player));
    test->next = NULL;
    Card *start = loadcards("src/data/cards.txt");
    _player *enemy_start = load_enemies("src/data/enemies.txt");
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
    printf("3\n");
    printf("%s\n", fight(test, enemy_start) ? "win": "loss");
    printf("3\n");
    free_enemy(enemy_start);
    free_card(start);
    free(test);
}*/

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
    for (int i = 0; enemy->deck[i] != NULL; i++)
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
        printf("-----------------------------------------------------\n");
        printf("PLAYER - Oxygen: %i/%i  Energy: %i/%i   Health: %i/%i\n", player->oxygen, player->maxOxygen, player->energy, player->maxEnergy, player->health, player->maxHealth);
        printf("%s - Oxygen: %i/%i  Energy: %i/%i   Health: %i/%i\n", enemy->name, enemy->oxygen, enemy->maxOxygen, enemy->energy, enemy->maxEnergy, enemy->health, enemy->maxHealth);
        printf("PLAYER HAND:\n1. %s\n2. %s\n3. %s\n", hand[0]->name, hand[1]->name, hand[2]->name);
        //Gets input from the player in the form of a string
        printf("Action>");
    	char input[15];
    	memset(&input[0], 0, sizeof(input));
    	fgets(input, 15, stdin);
    	int q;
    	for (q = 0; input[q] != '\n'; input[q] = tolower(input[q]), q++);
    	input[q] = '\0';
    	//Analyzes player input as one of 4 commands
    	int num[] = {2, 5, 3, 2, 3};
    	char *strings[] = {
    		"help", "h",
    		"quit", "q", "surrender", "stop", "s",
    		"info", "card", "i",
    		"play", "p",
    		"wand", "use", "w"
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
    	        printf("List of commands:\nhelp - this\nstop - leaves battle\ninfo - gives info on a card\nplay - plays a card\nitem - uses a potion\n");
    	        break;
    	    case 1: /*surrender*/
    	        return false;
    	        break;
    	    case 2: /*info*/
    	        //asks player to input the name of one of their cards, printing that card's information
    	        printf("Which card would you like to know about?\n>");
    	        fgets(input, 15, stdin);
    	        for (q = 0; input[q] != '\n'; q++);
    	        input[q] = '\0';
    	        for (int i = 0; i < 3; i++)
    	        {
    	            if (!strcmp(input, hand[i]->name)|| atoi(input) == i + 1)
    	            {
    	                print_card(hand[i]);
    	            }
    	        }
    	        break;
    	    case 3: /*play*/
    	        //asks player for the name of one of their cards
    	        printf("Which card would you like to play? (1, 2 or 3)\n>");
    	        fgets(input, 15, stdin);
    	        for (q = 0; input[q] != '\n'; q++);
    	        input[q] = '\0';
    	        for (int i = 0; i < 3; i++)
    	        {
    	            if (!strcmp(input, hand[i]->name) || atoi(input) == i + 1)
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
    	   case 4:
    	        if (player->wand->uses != 0)
    	        {
        	        printf("Player used an item to cast %s\n", player->wand->spell->name);
        	        play_card(player->wand->spell, enemy_cards[enemy_card_num], player, enemy);
        	        player->wand->uses--;
        	        enemy_card_num++;
        	        if (enemy_cards[enemy_card_num] == NULL)
        	        {
        	            shuffle(enemy_cards);
        	            enemy_card_num = 0;
        	        }
    	        }
    	        break;

    	   default:
    	       printf("Invalid Command\n");
    	       break;
    	}
    	result = check_life(player, enemy);
    	still_going = result == 0;
    }
    enemy->oxygen = enemy->maxOxygen;
    enemy->energy = enemy->maxEnergy;
    enemy->health = enemy->maxHealth;
    if (player->wand != NULL)
    {
        player->wand->uses++;
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
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        Card *temp = c[i];
        c[i] = c[j];
        c[j] = temp;
    }
}

void print_card(Card *current)
{
    printf("Name: %s\n", current->name);
    for (int i = 0; i < 7; i++)
    {
        printf("%s:", name_of_var_for_print_f[i]);
        printf("%i\n", (&(current->cost_o))[i]);
    }
    printf("Effects:\n");
    if (current->effects[0])
    {
        printf("\tVulnerable\n");
    }
    if (current->effects[1])
    {
        printf("\tInvulnerable\n");
    }
    if (current->effects[2])
    {
        printf("\tThorns\n");
    }
    if (current->effects[3])
    {
        printf("\tBleeding\n");
    }
    if (current->effects[4])
    {
        printf("\tExplosive\n");
    }
    if (current->effects[5])
    {
        printf("\tTargeting\n");
    }
}

void play_card(Card *card, Card *enemy_card, _player *player, _player *enemy)
{
    printf("-----------------------------------------------------\n");
    int modifier = 1;
    if (enemy_card->effects[0] == true)
    {
        printf("You will do double damage this round due to enemy vulnerability\n");
        modifier *= 2;
    }
    if (enemy_card->effects[1] == true)
    {
        printf("You will do no damage this round\n");
        modifier *= 0;
    }
    if (player_targeting)
    {
        printf("You will do double damage this round\n");
        modifier *= 2;
    }
    if (enemy_card->effects[3])
    {
        printf("Enemy is now bleeding\n");
        player_bleeding = card->effects[3];
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
        printf("You lose 1 health due to blood loss\n");
        player->health = fmin(player->maxHealth, fmax(0, player->health - 1));
    }
    if (enemy_thorns && card->dam_h >= 0)
    {
        printf("You lose 1 health due to enemy thorns\n");
        player->health = fmin(player->maxHealth, fmax(0, player->health - 1));
    }
    modifier = 1;
    if (card->effects[0] == true)
    {
        printf("Enemy will do double damage this round due to your vulnerability\n");
        modifier *= 2;
    }
    if (card->effects[1] == true)
    {
        printf("Enemy will do no damage this round\n");
        modifier *= 0;
    }
    if (enemy_targeting)
    {
        printf("Enemy will do double damage this round\n");
        modifier *= 2;
    }
    player_thorns = card->effects[2];
    if (card->effects[3])
    {
        printf("Enemy is now bleeding\n");
        enemy_bleeding = card->effects[3];
    }
    if (card->effects[5])
    {
        printf("You line up the perfect shot for next round\n");
        player_targeting = card->effects[5];
    }
    if (card->effects[4] == true)
    {
        enemy->health = fmin(10, fmax(0, round((double) enemy->health / (double) enemy->oxygen)));
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
        printf("Enemy lose 1 health due to blood loss\n");
        enemy->health = fmin(enemy->maxHealth, fmax(0, enemy->health - 1));
    }
    if (player_thorns && enemy_card->dam_h >= 0)
    {
        printf("Enemy loses 1 health due to player thorns\n");
        enemy->health = fmin(enemy->maxHealth, fmax(0, enemy->health - 1));
    }
    if (enemy_card->effects[4] == true)
    {
        player->health = fmin(10, fmax(0, round((double) player->health / (double) player->oxygen)));
    }
    if (enemy_card->effects[5])
    {
        printf("Enemy lines up the perfect shot for next round\n");
        enemy_targeting = enemy_card->effects[5];
    }
    printf("Enemy played: %s\n", enemy_card->name);
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

int strcasecmp(const char *s1, const char *s2)
{
  const unsigned char *p1 = (const unsigned char *) s1;
  const unsigned char *p2 = (const unsigned char *) s2;
  int result;
  if (p1 == p2)
    return 0;
  while ((result = tolower(*p1) - tolower(*p2++)) == 0)
    if (*p1++ == '\0')
      break;
  return result;
}