#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include "src/sat_gen.h"

struct satalite *sat;

int main(int argc, char *argv[])
{
    if (argc != 2) // check if we have the right amount of aruments
    {
        printf("please have a argument\n");
        return 0;
    }
    signal(SIGINT, INThandler);
    //clear screen
	printf("\033[2J\033[1;1H");
    printf("Creating satalite\n");
    sat = sat_gen(atoi(argv[1]), time(0));
    //set the room that the player is in to the starting room
    struct room *player_room = sat->starting_room;
    //make that room visable
   	player_room->seen = true;

    int i;
    bool running = true;
    while (running)
    {
    	//get input
		switch(get_command())
		{
			//process commands
			case 1://quit
				running = false;
				break;
			case 2://map
				draw_seen_map(sat);
				break;
			case 3://move
				move(&player_room, sat);
				break;
			case 4://devmap
				draw_map(sat->map);
				break;
		}
    }
    //free things when done
    printf("Freeing things\n");
    free_sat(sat);
    printf("Things have been freed\n");
}

void INThandler(int sig)
{
	//catch ctrl-c
	signal(sig, SIG_IGN);
    //free things when done
    printf("\nFreeing things\n");
    free_sat(sat);
    printf("Things have been freed\n");
    exit(0);
}
