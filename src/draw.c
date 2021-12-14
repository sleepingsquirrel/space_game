#include <stdio.h>

#include "main.h"
//array of colors
const char *colors[] = {"\033[1;39;49m\0", "\033[1;30;47m\0", "\033[1;30;46m\0", "\033[1;30;45m\0", "\033[1;30;44m\0", "\033[1;30;43m\0", "\033[1;30;42m\0", "\033[1;30;41m\0"};

void draw_map(uint8_t map[HEIGHT][WIDTH])
{
    printf("   ");
	//print numbers at top
    for (int x = 0; x < WIDTH; x++)
	{
		//alternate between black background and white background
		printf("%s%i%i",(x%2) ? "\033[1;39;49m\0" : "\033[1;30;47m\0", x/10, x%10);
	}
    printf("\n");
    for (int y = 0; y < HEIGHT; y++)
    {
		//print numbers on the side
        printf("%i%i ", y/10, y%10);
		//print things on the map
        for (int x = 0; x < WIDTH; x++)
        {
            printf("%s%c ", color((int) map[y][x]));
        }
        printf("\n");
    }
	//reset color
    printf("%s", colors[0]);
}

void draw_seen_map(satalite *sat)
{
	//gen map
	uint8_t map[HEIGHT][WIDTH][2];
	for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
		{
			map[y][x][0] = 0;
			map[y][x][1] = ' ';
		}
	// for (room_type *current = sat->room_types; current != NULL; current = current->next)
	// {
	// 	printf("%s%s\n", color(current->id < 0 ? 8 : current->id + 9), current->name);
	// }
	//draw seen rooms to it
	for (room *current = sat->rooms; current != NULL; current = current->next)
		if (current->seen)
		{
			for (int rx = 0; rx < current->w; rx++)
				for (int ry = 0; ry < current->h; ry++)
				{
					map[ry + current->y][rx + current->x][0] = current->data;//current->type->id < 0 ? 1 : current->type->id + 2;
				}
			map[current->y][current->x][1] = current->type->name[0];
		}
	//draw that map
	printf("   ");
	//print numbers at top
	for (int x = 0; x < WIDTH; x++)
	{
		//alternate between black background and white background
		printf("%s%i%i",(x%2) ? "\033[1;39;49m\0" : "\033[1;30;47m\0", x/10, x%10);
	}
	printf("\n");
	for (int y = 0; y < HEIGHT; y++)
	{
		//print numbers on the side
		printf("%i%i ", y/10, y%10);
		//print things on the map
		for (int x = 0; x < WIDTH; x++)
		{
			printf("%s%c ", color((int) map[y][x][0]), map[y][x][1]);
		}
		printf("\n");
	}
	//reset color
	printf("%s", colors[0]);
}

const char *color(int num)
{
	//get color from number
    return colors[num ? (num + 6) % 7 + 1 : 0];
}
