#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include "Sat_gen_c/sat_gen.h"

void see_room(struct room *rm);
int get_command();
char input[50];

int main(int argc, char *argv[])
{
    if (argc != 3) // check if we have the right amount of aruments
    {
        printf("please have a argument\n");
        return 0;
    }
	printf("\033[2J\033[1;1H");
    printf("Creating satalite\n");
    struct satalite *sat = sat_gen(atoi(argv[1]), atoi(argv[2]));
    printf("Printing satalite\n");
    draw_map(sat->map);
    struct room *player_room = sat->starting_room;
	see_room(player_room);
    int i;
    while (true)
    {
		switch(get_command())
		{
			case 1://quit
				goto LOOP;
			case 2://map
				draw_seen_map(sat);
			case 3://move
				break;
			case 4:
				for (struct door *current = player_room->doors; current != NULL; current = current->next)
				{
					printf("%p %i %c\n", current, current->doorp->data, current->direction);
				}
		}
    }
LOOP:
    printf("Freeing things\n");
    free_sat(sat);
    printf("Things have been freed\n");
}

void see_room(struct room *rm)
{
	rm->seen = true;
	for (struct door *current = rm->doors; current != NULL; current = current->next)
	{
		current->doorp->seen = true;
	}
}

int get_command()
{
	printf(">");
	fgets(input, 50, stdin);
	printf("\033[2J\033[1;1H");
	int i;
	for (i = 0; input[i] != '\n'; input[i] = tolower(input[i]), i++);
	input[i] = '\0';

	int num[] = {4, 2, 3, 2};
	int numpos = 1;
	char *strings[] = {
		"quit", "q", "close", "stop",
		"map", "draw",
		"move", "m", "mv",
		"door", "doors"};
	i = 0;
	for (int curr = 0; i < sizeof(strings) / sizeof(char*); i++, curr++)
	{
		if (curr == num[numpos-1])
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
