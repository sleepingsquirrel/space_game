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

int main(int argc, char *argv[])
{
    player = malloc(sizeof(_player));
    room_type_start = load_room_types("src/data/room_types.txt");
	for (room_type *current = room_type_start; current != NULL; current = current->next);
    signal(SIGINT, INThandler);
    //clear screen
	printf("\033[2J\033[1;1H");
	player->gold = 0;
    printf("Creating satalite\n");
	player->sat = sat_gen(fmax(atoi(argv[1] ? argv[1] : "0"), 1), time(0), room_type_start);
    //set the room that the player is in to the starting room
    player->room = player->sat->starting_room;
    //make that room visable
   	player->room->seen = true;
    int i;
    FILE *file;
    char buffer;
    bool running = true;
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
				move(&player->room, player->sat);
				break;
			case 4://devmap
				draw_map(player->sat->map);
				break;
			case 5://search
				search(player);
				break;
			case 6:
			    free_sat(player->sat);
			    player->sat = sat_gen(fmax(atoi(argv[1] ? argv[1] : "0"), 1), time(0), room_type_start);
			    printf("%i\n", player->sat->rooms_num);
			    draw_map(player->sat->map);
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
    printf("\nFreeing things\n");
    free_sat(player->sat);
    free_room_types(room_type_start);
    free(player);
    printf("Things have been freed\n");
    exit(0);
}
