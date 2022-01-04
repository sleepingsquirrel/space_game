#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#ifdef WIN64
	#include <conio.h>
#else
	#include <curses.h>
#endif

#include "src/main.h"

struct _player *player;
room_type *room_type_start;
WINDOW * mainwin;

int main(int argc, char *argv[])
{
	mainwin = initscr();
    player = malloc(sizeof(_player));
    room_type_start = load_room_types("src/data/room_types.txt");
    signal(SIGINT, INThandler);
    //clear screen
	printf("\033c");
	player->gold = 0;
    printf("Creating satalite\n");
	player->sat = sat_gen(fmax(atoi(argv[1] ? argv[1] : "0"), 1), time(0), room_type_start);
    //set the room that the player is in to the starting room
    player->room = player->sat->starting_room;
	player->x = player->room->x + player->room->w;
	player->y = player->room->y + player->room->h;
    //make that room visable
   	player->room->seen = true;
    FILE *file;
    char buffer;
    bool running = true;
	char input[BUFFER_SIZE];
    printf("Type \"help\" for help\n");
    while (running)
    {
    	//get input
		draw_move(player);
		memset(&(input[0]), 0, sizeof(input));
		printf(">");
		refresh();
		for (int i = 0; ; i++)
		{
		 	buffer = getch();
			if (buffer == '\033')
			{
				getch();
				printf("%c\n", getch());
			}
			if (buffer == '\b')
			{
				i -= 2;
			}
			if (i < BUFFER_SIZE - 1)
			{
				input[i] = buffer;
				if (input[i] == '\n')
				{
					input[i + 1] = '\0';
				}
			}
		}
		switch(get_command(input))
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
				// move(player, player->sat);
				break;
			case 4://devmap
				draw_map(player->sat->map);
				break;
			case 5://search
				search(player);
				break;
			case 6://new
				char intput[5];
				int inp;
				printf("\nGive Size: ");
				fgets(intput, 5, stdin);
				inp = atoi(intput);
			    free_sat(player->sat);
			    player->sat = sat_gen(fmin(inp > 1 ? inp : 1, 50), time(0), room_type_start);
			    printf("%i\n", player->sat->rooms_num);
			    draw_map(player->sat->map);
				player->room = player->sat->starting_room;
				player->x = player->room->x + player->room->w;
				player->y = player->room->y + player->room->h;
				player->room->seen = true;
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
	delwin(mainwin);
	endwin();
	refresh();
    exit(0);
}
