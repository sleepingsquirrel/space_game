#include <stdio.h>
#include <string.h>
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
            printf("%s  ", color((int) map[y][x]));
        }
        printf("\n");
    }
	//reset color
    printf("%s", colors[0]);
}

void draw_move(_player *player)
{
	for (room *current = player->sat->rooms; current != NULL; current = current->next)
	{
		current->drawn = false;
	}
	uint8_t map[MOVEH][MOVEW];
	memset(&(map[0][0]), 0, sizeof(map));
	draw_close_seen_rooms_to_map(player, player->room, map);
	for (int y = 0; y < MOVEH; y++)
	{
		//print things on the map
		for (int x = 0; x < MOVEW; x++)
		{
			printf("%s%c ", color(map[y][x]), (y == MOVEH / 2 && x == MOVEW / 2) ? 'p' : ' ');
		}
		printf("\n");
	}
	printf("%s\n", color(0));
	for (room *current = player->sat->rooms; current != NULL; current = current->next)
	{
		current->drawn = false;
	}
}

void draw_close_seen_rooms_to_map(_player *player, room *location, uint8_t map[MOVEH][MOVEW])
{
	location->drawn = true;
	for (int rx = 0; rx < location->w; rx++)
		for (int ry = 0; ry < location->h; ry++)
			if (ry + location->y - player->y + MOVEH / 2 + 1 >= 0 && ry + location->y - player->y + MOVEH / 2 + 1< MOVEH && rx + location->x - player->x + MOVEH / 2 + 1>= 0 && rx + location->x - player->x + MOVEW / 2 + 1 < MOVEW)
			{
				map[ry + location->y - player->y + MOVEH / 2 + 1][rx + location->x - player->x + MOVEW / 2 + 1] = location->data;//current->type->id < 0 ? 1 : current->type->id + 2;
			}
	for (door *current = location->doors; current != NULL; current = current->next)
		if (!current->doorp->drawn)
			if (!(current->doorp->y - player->y + MOVEH / 2 + 1 >= MOVEH && current->doorp->x - player->x + MOVEW / 2 + 1>= MOVEW && current->doorp->y + current->doorp->h - player->y + MOVEH / 2 + 1< 0, current->doorp->x + current->doorp->w - player->x + MOVEW / 2 + 1 < 0))
				if (current->doorp->seen)
				{
					draw_close_seen_rooms_to_map(player, current->doorp, map);
				}
				else if (current->y - player->y + MOVEH / 2 + 1>= 0 && current->y - player->y + MOVEH / 2 + 1 < MOVEH && current->x - player->x + MOVEW / 2 + 1 >= 0 && current->x - player->x + MOVEW / 2 + 1 < MOVEW)
				{
					map[current->y - player->y + MOVEH / 2 + 1][current->x - player->x + MOVEW / 2 + 1] = current->doorp->data;
				}
}

void draw_seen_map(satalite *sat)
{
	//gen map
	int minx, miny, maxx, maxy;
	minx = WIDTH;
	miny = HEIGHT;
	maxx = 0;
	maxy = 0;
	for (room *current = sat->rooms; current != NULL; current = current->next)
		if (current->seen)
		{
			if (current->x < minx)
			{
				minx = current->x;
			}
			if (current->y < miny)
			{
				miny = current->y;
			}
			if (current->x + current->w > maxx)
			{
				maxx = current->x + current->w;
			}
			if (current->y + current->h > maxy)
			{
				maxy = current->y + current->h;
			}
		}
	minx--;
	uint8_t map[maxy-miny][maxx-minx][2];
	for (int x = 0; x < maxx-minx; x++)
		for (int y = 0; y < maxy-miny; y++)
		{
			map[y][x][0] = 0;
			map[y][x][1] = (int)' ';
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
					map[ry + current->y - miny][rx + current->x - minx][0] = current->data;//current->type->id < 0 ? 1 : current->type->id + 2;
				}
			map[current->y - miny][current->x - minx][1] = current->type->name[0];

		}
	//draw that map
	//print numbers at top
	printf("\n");
	for (int y = 0; y < maxy-miny; y++)
	{
		//print things on the map
		for (int x = 0; x < maxx-minx; x++)
		{
			printf("%s%c ", color((int) map[y][x][0]), map[y][x][1]);
		}
		printf("%s\n", color(0));
	}
	//reset color
	printf("%s\n", color(0));
}

const char *color(int num)
{
	//get color from number
    return colors[num ? (num + 6) % 7 + 1 : 0];
}
