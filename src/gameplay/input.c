#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "../main.h"

void move(_player *player, satalite *sat)
{
	//gen map and set it to 0
    uint8_t map[HEIGHT][WIDTH];
    for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			map[y][x] = 0;
	//draw the room the player is in to the map
	for (room *current = player->sat->rooms; current != NULL; current = current->next)
		if (current->seen)
		{
		    for (int rx = 0; rx < current->w; rx++)
				for (int ry = 0; ry < current->h; ry++)
				{
					map[ry + current->y][rx + current->x] = current->data;
				}
			//draw rooms
		    for (door *current_door = current->doors; current_door != NULL; current_door = current_door->next)
		    {
		        map[current_door->y][current_door->x] = current_door->doorp->data;
		    }
		}
	//print out map but just the part we have drawn
    printf("\n");
    for (int ry = 0; ry  < player->room->h + MOVEH; ry++)
    {
    	printf("  ");
        for (int rx = 0; rx < player->room->w + MOVEW; rx++)
		{
			//test if the current position is the data of the players position or 0
				//print color without index
            	printf("%s  ", color(map[player->room->y-(MOVEH / 2) + ry][player->room->x-(MOVEW / 2) + rx]));
				//generate index
				int i = 1;
                for (door *current = player->room->doors; current != NULL; current = current->next, i++)
                    if (current->x == player->room->x-(MOVEW / 2) + rx && current->y == player->room->y-(MOVEH / 2) + ry)
                    {
						printf("\b\b%i ", i);
                        break;
                    }
				//print color with index
        }
		printf("%s\n", color(0));
	}
	//get input
	printf("\nmove to>");
	char input[3];
	int inp = 0;
	fgets(input, 3, stdin);
	//convert input into integer
	if (isdigit(input[0]))
	{
		inp += input[0] - '0';
		if (isdigit(input[1]))
		{
			inp *= 10;
			inp += input[1] - '0';
		}
	}
	else
	{
		//if it start is not int return
		printf("no valid door selected\n");
		return;
	}
	//get the specified room
	door *current = player->room->doors;
	for (int i = 0; current != NULL && i+1 < inp; current = current->next, i++);
	//prevent segmentation fault
	if (current == NULL)
	{
		printf("no valid door selected\n");
		return;
	}
	//update player pos
	player->room = current->doorp;
	//see room
	//TODO: add random encounters
	bool chance = (bool) (rand() % 2);
	if (!player->room->seen)
	{
		player->room->seen = true;
		if (chance)
		{
			if(!encounter(player))
			{
				Kill();
			}
		}
	}
	room_effects(player);
	printf("Entered %s\n", player->room->type->name);
	printf("You enter %s\n", player->room->type->descriptions[0]);
}

int last_command = 0;
int get_command()
{
    char input[51];
	//get input
	printf(">");
	fgets(input, 50, stdin);
	//clear screen
	printf("\033c");
	//make input lowercase
	int i;
	for (i = 0; input[i] != '\n'; input[i] = tolower(input[i]), i++);
	//end the string
	input[i] = '\0';
	if (input[0] == '\t')
	{
		return last_command;
	}
	//num stores nuber of aliases of each command
	int num[] = {3, 4, 3, 3, 5, 3};
	//strings stores each command
	char *strings[] = {
		"help", "h", "\"help\"",
		"quit", "q", "close", "stop",
		"map", "draw", "mp",
		"move", "m", "mv",
		"search", "s", "see", "inspect", "room",
		"i", "inc", "iventory"
	};
	int numpos = 0;
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
			last_command = numpos;
			return numpos;
		}
	}
	printf("invalid command\n");
	return -1;
}

void search(_player *player)
{
	//loot
	printf("%s\n", player->room->type->name);
}
