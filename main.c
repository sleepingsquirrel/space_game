#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "Sat_gen_c/sat_gen.h"

void see_room(struct room *rm);
int get_command();
char input[51];

int main(int argc, char *argv[])
{
    if (argc != 2) // check if we have the right amount of aruments
    {
        printf("please have a argument\n");
        return 0;
    }
    //clear screen
	printf("\033[2J\033[1;1H");
    printf("Creating satalite\n");
    struct satalite *sat = sat_gen(atoi(argv[1]), time(0));
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
				break;
			case 4: //print doors of current room
				for (struct door *current = player_room->doors; current != NULL; current = current->next)
				{
					printf("%p %i %c\n", current, current->doorp->data, current->direction);
				}
				break;
			case 5://devmap
				draw_map(sat->map);
				break;
		}
    }
    //free things when done
    printf("Freeing things\n");
    free_sat(sat);
    printf("Things have been freed\n");
}

int get_command()
{
	//get input
	printf(">");
	fgets(input, 50, stdin);
	//clear screen
	printf("\033[2J\033[1;1H");
	//make input lowercase
	int i;
	for (i = 0; input[i] != '\n'; input[i] = tolower(input[i]), i++);
	//end the string
	input[i] = '\0';

	//num stores nuber of aliases of each command
	int num[] = {4, 2, 3, 2, 1};
	//strings stores each command
	char *strings[] = {
		"quit", "q", "close", "stop",
		"map", "draw",
		"move", "m", "mv",
		"door", "doors",
		"devmap"
	};
	int numpos = 1;
	i = 0;
	for (int curr = 0; i < sizeof(strings) / sizeof(char*); i++, curr++)
	{
		//see if we have reached end of command aliases
		if (curr == num[numpos])
		{
			numpos++;
			curr = 0;
		}

		if (!strcmp(input, strings[i]))
		{
			printf("%s %i\n", strings[i], numpos);
			return numpos;
		}
	}
	return 0;
}
