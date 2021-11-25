#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "sat_gen.h"


void move(struct room *player_room, struct satalite *sat)
{
    uint8_t map[HEIGHT][WIDTH];
    for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			map[y][x] = 0;
    int minx = player_room->x-1;
    int miny = player_room->y-1;
    int maxx = player_room->x + player_room->w+1;
    int maxy = player_room->y + player_room->h+1;
    for (int rx = 0; rx < player_room->w; rx++)
		for (int ry = 0; ry < player_room->h; ry++)
		{
			map[ry + player_room->y][rx + player_room->x] = player_room->data;
		}
    for (struct door *current = player_room->doors; current != NULL; current = current->next)
    {
        map[current->y][current->x] = current->doorp->data;
    }
    int index = 1;
    for (int ry = 0; ry  < player_room->h + 2; ry++)
        {
        for (int rx = 0; rx < player_room->w + 2; rx++)
            if (map[player_room->y-1 + ry][player_room->x-1 + rx] == player_room->data ||
                map[player_room->y-1 + ry][player_room->x-1 + rx] == 0)
            {
               printf("%s  ", color(map[player_room->y-1 + ry][player_room->x-1 + rx]));
            }
            else
            {
                for (struct door *current = player_room->doors; current != NULL; current = current->next, index++)
                    if (current->x == player_room->x-1 + rx && current->y == player_room->y-1 + ry)
                    {
                        break;
                    }
                printf("%s%i ", color(map[player_room->y-1 + ry][player_room->x-1 + rx]), index);
                index = 1;
            }
        printf("\n");
        }
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
	int num[] = {4, 2, 3, 2, 1};
	//strings stores each command
	char *strings[] = {
		"quit", "q", "close", "stop",
		"map", "draw",
		"move", "m", "mv",
		"door", "doors",
		"devmap"
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
			printf("%s %i\n", strings[i], numpos);
			return numpos + 1;
		}
	}
	return 0;
}
