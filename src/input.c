#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "sat_gen.h"

void move(room **player, satalite *sat)
{
	room *player_room = *player;
	//gen map and set it to 0
    uint8_t map[HEIGHT][WIDTH];
    for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			map[y][x] = 0;
	//draw the room the player is in to the map
    for (int rx = 0; rx < player_room->w; rx++)
		for (int ry = 0; ry < player_room->h; ry++)
		{
			map[ry + player_room->y][rx + player_room->x] = player_room->data;
		}
	//draw rooms
    for (door *current = player_room->doors; current != NULL; current = current->next)
    {
        map[current->y][current->x] = current->doorp->data;
    }
	//print out map but just the part we have drawn
    int index = 1;
    for (int ry = 0; ry  < player_room->h + 2; ry++)
        {
        for (int rx = 0; rx < player_room->w + 2; rx++)
			//test if the current position is the data of the players position or 0
            if (map[player_room->y-1 + ry][player_room->x-1 + rx] == player_room->data ||
                map[player_room->y-1 + ry][player_room->x-1 + rx] == 0)
            {
				//print color without index
            	printf("%s  ", color(map[player_room->y-1 + ry][player_room->x-1 + rx]));
            }
            else
            {
				//generate index
                for (door *current = player_room->doors; current != NULL; current = current->next, index++)
                    if (current->x == player_room->x-1 + rx && current->y == player_room->y-1 + ry)
                    {
                        break;
                    }
				//print color with index
                printf("%s%i ", color(map[player_room->y-1 + ry][player_room->x-1 + rx]), index);
				//reset index
				index = 1;
            }
        printf("\n");
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
	door *current = player_room->doors;
	for (int i = 0; current != NULL && i+1 < inp; current = current->next, i++);
	//prevent segmentation fault
	if (current == NULL)
	{
		printf("no valid door selected\n");
		return;
	}
	//update player pos
	(*player) = current->doorp;
	//see room
	(*player)->seen = true;
	//TODO: add random encounters

	printf("moved suceccfully\n");
}

int get_command()
{
    char input[51];
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
	int num[] = {2, 4, 3, 3, 1, 5};
	//strings stores each command
	char *strings[] = {
		"help", "h",
		"quit", "q", "close", "stop",
		"map", "draw", "mp",
		"move", "m", "mv",
		"devmap",
		"search", "s", "see", "inspect", "room"
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
			return numpos;
		}
	}
	printf("invalid command\n");
	return -1;
}

void search(_player *player)
{
	printf("%s\n", player->player_room->room_type->name);
}
