#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sat_gen.h"

const char *colors[] = {"\033[1;39;49m\0", "\033[1;37;47m\0", "\033[1;37;46m\0", "\033[1;37;45m\0", "\033[1;37;44m\0", "\033[1;37;43m\0", "\033[1;37;42m\0", "\033[1;37;41m\0"};

struct satalite *sat_gen(int size, int seed)
{
    srand(seed);
    struct satalite *sat = malloc(sizeof(struct satalite));
    sat->rooms = NULL;
    sat->rooms_num = 0;
    sat->doors_num = 0;
    sat->sat_size = size;
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            sat->map[y][x] = 0;
        }
    gen_map(sat);
    for (struct room *current = sat->rooms; current != NULL; current = current->next)
    {
        make_doors(current);
    }
    return sat;
}

struct room *room_gen(int x1, int y1, int w1, int h1, int roomNum1, struct satalite *p)
{
    struct room *roomp = malloc(sizeof(struct room));
    roomp->x = x1;
    roomp->y = y1;
    roomp->w = w1;
    roomp->h = h1;
    roomp->doors = NULL;
    roomp->seen = false;
    roomp->data = roomNum1;
    roomp->parent = p;
    roomp->next = roomp->parent->rooms;
    roomp->parent->rooms = roomp;
    roomp->parent->rooms_num++;
    return roomp;
}

void door_gen(struct room *parent, struct room *doorp, char direction)
{
    struct door *p = malloc(sizeof(struct door));
    p->direction = direction;
    p->doorp = doorp;
    p->next = parent->doors;
    parent->doors = p;
    parent->parent->doors_num++;
}

void gen_map(struct satalite *sat) //generate satalite
{
    //draw center room of the satalite
    int tempranx = (1 + rand() % 10) / 4 + 2;
    int temprany = (1 + rand() % 10) / 8 + 2;
    sat->starting_room = draw_to_map(25 - tempranx / 2, 12 - temprany / 2, tempranx, temprany, 1, sat);
    for (int size = 1; size < sat->sat_size && sat->rooms_num < sat->sat_size; size++)
        for (int i = 2; i < size + 2; i++)
            for (int y = 0; y < HEIGHT; y++)
                for (int x = 0; x < WIDTH; x++)
                    if (sat->map[y][x] == i - 1 && rand() % 3 == 0)
                    {
                        tempranx = (size - i) / 3 + rand() % 5;
                        temprany = (size - i) / 3 + rand() % 5;
                        if (sat->map[y + 1][x] == 0)
                        {
                            draw_to_map(x - tempranx / 4, y + 1, fmax(tempranx / 2, 1), fmax(temprany, 1), i, sat);
                        }
                        if (sat->map[y - 1][x] == 0)
                        {
                            draw_to_map(x - tempranx / 4, y - temprany, fmax(tempranx / 2, 1), fmax(temprany, 1), i, sat);
                        }
                        if (sat->map[y][x - 1] == 0)
                        {
                            draw_to_map(x - tempranx, y - temprany / 4, fmax(tempranx, 1), fmax(temprany / 2, 1), i, sat);
                        }
                        if (sat->map[y][x + 1] == 0)
                        {
                            draw_to_map(x + 1, y - temprany / 4, fmax(tempranx, 1), fmax(temprany / 2, 1), i, sat);
                        }
                        if (sat->rooms_num >= sat->sat_size)
                        {
                            return;
                        }
                    }
}

bool check_empty(int x, int y, int w, int h, int data, struct satalite *sat)
{
    for (int rx = -1; rx <= w; rx++)
        for (int ry = -1; ry <= h; ry++)
            if (!((rx < 0 || ry < 0 || rx == w || ry == h) && sat->map[ry + y][rx + x] == data - 1) && sat->map[ry + y][rx + x] != 0)
            {
                return false;
            }
    return true;
}

const char *color(int num)//get color from number
{
    return colors[num ? (num + 6) % 7 + 1 : 0];
}

struct room *draw_to_map(int x, int y, int w, int h, int data, struct satalite *sat)
{
    if (x > 0 && y > 0 && x + w < WIDTH && y + h < HEIGHT)
        if (check_empty(x, y, w, h, data, sat))
        {
            for (int rx = 0; rx < w; rx++)
                for (int ry = 0; ry < h; ry++)
                {
                    sat->map[ry + y][rx + x] = data;
                }
            return room_gen(x, y, w, h, data, sat);
        }
    return NULL;
}

void draw_map(uint8_t map[HEIGHT][WIDTH])
{
    printf("   ");
    for (int x = 0; x < WIDTH; x++)
        printf("%s%i%i",(x%2) ? "\033[1;39;49m\0" : "\033[1;30;47m\0", x/10, x%10);
    printf("\n");
    for (int y = 0; y < HEIGHT; y++)
    {
        printf("%i%i ", y/10, y%10);
        for (int x = 0; x < WIDTH; x++)
        {
            printf("%s  ", color((int) map[y][x]));//, (char)(sat->map[y][x] + '0'));
        }
        printf("\n");
    }
    printf("%s", colors[0]);
}

void draw_seen_map(struct satalite *sat)
{
	uint8_t map[HEIGHT][WIDTH];
	for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			map[y][x] = 0;
	for (struct room *current = sat->rooms; current != NULL; current = current->next)
		if (current->seen)
			for (int rx = 0; rx < current->w; rx++)
				for (int ry = 0; ry < current->h; ry++)
				{
					map[ry + current->y][rx + current->x] = current->data;
				}
	draw_map(map);
}

struct room *find_room(int x, int y, struct satalite *sat)
{
    for (struct room *current = sat->rooms; current != NULL; current = current->next)
        if (current->x <= x && current->y <= y && current->x + current->w > x && current->y + current->h > y)
        {
            return current;
        }
    return NULL;
}

void make_doors(struct room *p)
{
    bool isin;
    struct room *doorp;
    for (int rx = -1; rx < p->w + 1; rx++)
        for (int ry = -1; ry < p->h + 1; ry++)
            if (p->parent->map[p->y + ry][p->x + rx] != p->data && p->parent->map[p->y + ry][p->x + rx] != 0)
            {
                isin = false;
                doorp = find_room(p->x + rx, p->y + ry, p->parent);
                for (struct door *current = p->doors; current != 0; current = current->next)
                    if (current->doorp == doorp)
                    {
                        isin = true;
                    }
                if (abs(rx) == abs(ry) && p->data == 1) {
                    printf("andnskj %i %i\n", rx+p->x, ry+p->y);
                }
                if (!isin && p->data != doorp->data) //&& !(abs(rx) == abs(ry)))
                {
                    if (rx == -1 || rx == p->w)
                        door_gen(p, doorp, (rx == -1) ? 'W' : 'E');
                    else
                    {
                        door_gen(p, doorp, (ry == -1) ? 'N' : 'S');
                    }
                }
            }
}

void free_sat(struct satalite *sat)
{
    free_rooms(sat->rooms);
    free(sat);
}

void free_rooms(struct room *p)
{
    free_doors(p->doors);
    if (p->next != NULL)
    {
        free_rooms(p->next);
    }
    free(p);
}

void free_doors(struct door *p)
{
    if (p == NULL)
    {
        return;
    }
    if (p->next != NULL)
    {
        free_doors(p->next);
    }
    free(p);
}
