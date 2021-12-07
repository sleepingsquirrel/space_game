#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main.h"


satalite *sat_gen(int level, int seed)
{
	//set seed
    srand(seed);
	//alloc sat
    satalite *sat = malloc(sizeof(satalite));
	//write attrubutes
    sat->rooms = NULL;
    sat->rooms_num = 0;
    sat->doors_num = 0;
    sat->sat_size = size;
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            sat->map[y][x] = 0;
        }
	//generate map and rooms
    gen_map(sat);
	//gererate doors
    for (room *current = sat->rooms; current != NULL; current = current->next)
    {
        make_doors(current);
    }
    return sat;
}

room *room_gen(int x1, int y1, int w1, int h1, int roomNum1, satalite *p)
{
	//alloc room
    room *roomp = malloc(sizeof(room));
	//write attrubutes
    roomp->x = x1;
    roomp->y = y1;
    roomp->w = w1;
    roomp->h = h1;
    roomp->doors = NULL;
    roomp->seen = false;
    roomp->data = roomNum1;
    roomp->parent = p;
	//add to linked list
    roomp->next = roomp->parent->rooms;
    roomp->parent->rooms = roomp;
    roomp->parent->rooms_num++;
    return roomp;
}

void door_gen(room *parent, room *doorp, char direction, int x, int y)
{
	//alloc door
    door *p = malloc(sizeof(door));
	//write attrubutes
    p->direction = direction;
    p->doorp = doorp;
    p->x = x;
    p->y = y;
	//add to linked list
    p->next = parent->doors;
    parent->doors = p;
    parent->parent->doors_num++;
}

void gen_map(satalite *sat) //generate satalite
{
    int tempranx = (1 + rand() % 10) / 4 + 2;
    int temprany = (1 + rand() % 10) / 8 + 2;
	//draw center room of the satalite and make it starting_room
    sat->starting_room = draw_to_map(25 - tempranx / 2, 12 - temprany / 2, tempranx, temprany, 1, sat);
	//gen the rooms
	for (int size = 1; size < sat->sat_size && sat->rooms_num < sat->sat_size; size++)
        for (int i = 2; i < size + 2; i++)
            for (int y = 0; y < HEIGHT; y++)
                for (int x = 0; x < WIDTH; x++)
					//make shure the room behind this one is one less than the this and a random factor
                    if (sat->map[y][x] == i - 1 && rand() % 3 == 0)
                    {
						//set rands
                        tempranx = (size - i) / 3 + rand() % 5;
                        temprany = (size - i) / 3 + rand() % 5;
						//make the room
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

bool check_empty(int x, int y, int w, int h, int data, satalite *sat)
{
	//for each point within rect make shure it is either 0 or data - 1
    for (int rx = -1; rx <= w; rx++)
        for (int ry = -1; ry <= h; ry++)
            if (!((rx < 0 || ry < 0 || rx == w || ry == h) && sat->map[ry + y][rx + x] == data - 1) && sat->map[ry + y][rx + x] != 0)
            {
                return false;
            }
    return true;
}

room *draw_to_map(int x, int y, int w, int h, int data, satalite *sat)
{
	//make shure rect is within map
    if (x > 0 && y > 0 && x + w < WIDTH && y + h < HEIGHT)
		//make shure it is empty
        if (check_empty(x, y, w, h, data, sat))
        {
			//write it
            for (int rx = 0; rx < w; rx++)
                for (int ry = 0; ry < h; ry++)
                {
                    sat->map[ry + y][rx + x] = data;
                }
			//return it
            return room_gen(x, y, w, h, data, sat);
        }
    return NULL;
}



room *find_room(int x, int y, satalite *sat)
{
	//for each room
    for (room *current = sat->rooms; current != NULL; current = current->next)
		//check if the point is inside
        if (current->x <= x && current->y <= y && current->x + current->w > x && current->y + current->h > y)
        {
            return current;
        }
    return NULL;
}

void make_doors(room *p)
{
    bool isin;
    room *doorp;
	//loop around rect
    for (int rx = -1; rx < p->w + 1; rx++)
        for (int ry = -1; ry < p->h + 1; ry++)
            if (p->parent->map[p->y + ry][p->x + rx] != p->data && p->parent->map[p->y + ry][p->x + rx] != 0)
            {
				//make shure door not in linked list already
                isin = false;
                doorp = find_room(p->x + rx, p->y + ry, p->parent);
                for (door *current = p->doors; current != NULL; current = current->next)
                    if (current->doorp == doorp)
                    {
                        isin = true;
                    }
				//make shure point is not in corner
                if (!isin && !((rx+1 == 0 || rx == p->w) && (ry+1 == 0 || ry == p->h)))
                {
					//add door with dirrection
                    if (rx == -1 || rx == p->w)
					{
						door_gen(p, doorp, (rx == -1) ? 'W' : 'E', rx+p->x, ry+p->y);
					}
                    else
                    {
                        door_gen(p, doorp, (ry == -1) ? 'N' : 'S', rx+p->x, ry+p->y);
                    }
                }
            }
}

void gen_specal_rooms(satalite *sat)
{
	for (room *current = sat->rooms; current != NULL; current = current->next);
}

void free_sat(satalite *sat)
{
	//recusivly free rooms
    free_rooms(sat->rooms);
    free(sat);
}

void free_rooms(room *p)
{
	//recusivly free doors
    free_doors(p->doors);
    if (p->next != NULL)
    {
		//recursivly free rooms
        free_rooms(p->next);
    }
	//free self
    free(p);
}

void free_doors(door *p)
{
	//stp segmentation falut
    if (p == NULL)
    {
        return;
    }
    if (p->next != NULL)
    {
		//recursivly free doors
        free_doors(p->next);
    }
	//free self
    free(p);
}
