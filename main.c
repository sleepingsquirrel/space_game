#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include "src/main.h"

struct _player *player;
room_type *room_type_start;
static Card *card_start;
static _player *enemy_start;

int main(int argc, char *argv[])
{
    player = malloc(sizeof(_player));
    memset(&player->items[0], 0, sizeof(player->items));
    memset(&player->deck[0], 0, sizeof(player->deck));
    player->wand = NULL;
    card_start = loadcards("src/data/cards.txt");
    enemy_start = load_enemies("src/data/enemies.txt");
    room_type_start = load_room_types("src/data/room_types.txt");
	for (room_type *current = room_type_start; current != NULL; current = current->next);
    signal(SIGINT, INThandler);
    //clear screen
	player->gold = 0;
    printf("Creating satalite\n");
	player->sat = sat_gen(player, fmax(atoi(argv[1] ? argv[1] : "0"), 1), time(0), room_type_start);
	for (int i = 0; i < 6; (&(player->health))[i] = 10, i++);
    //set the room that the player is in to the starting room
    player->room = player->sat->starting_room;
    //make that room visable
   	player->room->seen = true;
   	player->deck[0] = find_card("Kick");
   	player->deck[1] = find_card("Kick");
   	player->deck[2] = find_card("Energize");
   	player->deck[3] = find_card("Rest");
   	player->deck[4] = find_card("Wound");
   	player->deck[5] = find_card("Missile");
   	player->deck[6] = find_card("Reckless Jab");
   	player->deck[7] = find_card("Reckless Jab");
   	player->deck[8] = find_card("Oxygen Drain");
   	player->deck[9] = find_card("True Strike");
   	player->deck[10] = find_card("Cautious Attack");
   	player->deck[11] = find_card("Shield");
    int i;
    FILE *file;
    char buffer;
    bool running = true;
	char input[5];
	int inp;
	printf("Now entering %s...\n", player->sat->name);
	printf("You enter %s\n", player->room->type->descriptions[0]);
	room_effects(player);
    printf("Type \"help\" for help\n");
    while (running)
    {
    	//get input
		switch(get_command())
		{
			//process commands
			case 0://help
				file = fopen("help.txt", "r");
				while (fread(&buffer, sizeof(char), 1, file))
				{
					printf("%c", buffer);
				}
				fclose(file);
				break;
			case 1://quit
				running = false;
				break;
			case 2://map
				draw_seen_map(player->sat);
				break;
			case 3://move
				move(player, player->sat);
				break;
			case 5://search
				search(player);
				break;
			case 6:
				inventory(player);
				break;
		}
    }
    Kill();
}

void INThandler(int sig)
{
	//catch ctrl-c
	signal(sig, SIG_IGN);
    //free things when done
    Kill();
}

void Kill()
{
	printf("\n");
	printf("------------------\n");
	printf("|                |\n");
	printf("|   GAME   OVER  |\n");
	printf("|                |\n");
	printf("------------------\n\n");
    //printf("\nFreeing things\n");
    free_sat(player->sat);
    free_room_types(room_type_start);
    free_card(getCards());
    free_enemy(getEnemies());
    for (int i = 0; i < ITEMS; (player->items[i] != NULL) ? free_item(player->items[i]) : 0, i++);
    free(player);
    //printf("Things have been freed\n");
    exit(0);
}

Card *getCards()
{
	return card_start;
}

_player *getEnemies()
{
	return enemy_start;
}