#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "../main.h"
//                      'murica   europe    south america    russia(PSMLSMSSR)                                                          aliens
const char *factions[] = {"USGA", "UN", "U.S.F. L.A.F.", "Peoples States of Marxist-Leninist-Stalinist-Maoist Soviet Socalist Republics", "Unknown"};

satalite *sat_gen(_player *player, int level, int seed, room_type *room_types)
{
	//set seed
    srand(seed);
	//alloc sat
    satalite *sat = malloc(sizeof(satalite));
    sat->level = level;
    sat->plotted = false;
    sat->parent = player;
	//write attrubutes
	sat->seed = seed;
    float faction_weights[] = {level < 7 ?  0 : (float) level * 0.025 + 4,
    (float) level * 0.025 + 5,
    (float) level * -0.2 + 5,
    (float) level * -0.05 + 4,
    pow(2, level - 25) / 5};

    float smallest = 0;
    float curr;
    sat->faction = factions[0];
    for (int i = 0; i < sizeof(factions) / sizeof(char*); i++)
    {
        curr = faction_weights[i] * ((float) rand() / RAND_MAX);
        if (smallest <= curr)
        {
            smallest = curr;
            sat->faction = factions[i];
            sat->factionID = i;
        }
    }
    sat->name = sat_name(sat->factionID);
    sat->room_types = room_types;
    sat->rooms = NULL;
    sat->rooms_num = 0;
    sat->doors_num = 0;
	int min_rooms = 0;
	for (room_type *type = sat->room_types; type != NULL; type = type->next)
		if (type->min_num > 0)
		{
			min_rooms++;
		}
    sat->sat_size = min_rooms + fmax(0, level * 5 + rand() % 7 - 5);
    // printf("%i\n", sat->sat_size);
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            sat->map[y][x] = 0;
        }
	//generate map and rooms
    gen_map(sat);
    for (room *current = sat->rooms; current != NULL; current = current->next)
    {
        make_doors(current);
    }
	gen_room_types(sat);
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
    roomp->used = false;
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
	//malloc door
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
                        if (sat->map[y + 1][x] == 0)
                        {
                            draw_to_map(x - tempranx / 5, y + 1, fmax(tempranx / 2, 1), fmax(temprany, 1), i, sat);
                        }
                        if (sat->map[y - 1][x] == 0)
                        {
                            draw_to_map(x - tempranx / 5, y - temprany, fmax(tempranx / 2, 1), fmax(temprany, 1), i, sat);
                        }
                        if (sat->map[y][x - 1] == 0)
                        {
                            draw_to_map(x - tempranx, y - temprany / 5, fmax(tempranx, 1), fmax(temprany / 2, 1), i, sat);
                        }
                        if (sat->map[y][x + 1] == 0)
                        {
                            draw_to_map(x + 1, y - temprany / 5, fmax(tempranx, 1), fmax(temprany / 2, 1), i, sat);
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

void gen_room_types(satalite *sat)
{
	for (room_type *current = sat->room_types; current != NULL; current = current->next)
	{
		current->sat_has = 0;
// 		printf("%s %i\n", current->name, current->id);
	}
	for (room *current = sat->rooms; current != NULL; current = current->next)
	{
		current->type = rand_room_type(sat->room_types, sat);
	}
	for (room_type *current = sat->room_types; current != NULL; current = current->next)
	{
	    if (current->id == 0 || current->id == 2)
	        if (current->sat_has <= 0)
	        {
	           // printf("AAAAAAAAAAAAAAAAAAAAAAAAA\n");
	            gen_room_types(sat);
	        }
	    if (current->sat_has > 0)
	    {
	       // printf("%s\n", current->name);
	       // printf("%s %i, %f\n", current->name, current->sat_has, (float)sat->rooms_num) / (float)current->sat_has;
	    }
	}
}

room_type *rand_room_type(room_type *start, satalite *sat)
{
    //random number between 1 & 100
	int probabilaty = 0;
	for (room_type *current = start; current != NULL; current = current->next)
	{
		probabilaty += current->probabilaty;
	}
	int randnum = rand() % probabilaty;
	//for each room_type
	for (room_type *current = start; current != NULL; current = current->next)
	{
		// printf("%s\n", current->name);
		randnum -= current->probabilaty;
		if (randnum < 0)
		{
			if (current->sat_has || current->min_num == 0)
			{
				if (current->id == 0 || current->id == 2)
				{
					randnum = rand() % 100;
					current = start;
					continue;
				}

				if (current->id == -1)
				{
				    room_type *next = start;
				    for (int i = 0; i < 4 - sat->factionID; i++, next = next->next);
                    // printf("%s, %s\n", next->name, sat->faction);
                    current->sat_has++;
                    return next;
				}

				for (room_type *type = start; type != NULL; type = type->next)
					if (type->min_num > 0 && !type->sat_has)
					{
						randnum = rand() % 100;
						current = start;
					}
				if (randnum >= 0)
				{
				    continue;
				}

				current->sat_has++;
				return current;
			}
			else
			{
				current->sat_has++;
				return current;
			}
		}
	}
	printf("Something went wrong\n");
	return NULL;
}

void free_sat(satalite *sat)
{
	//recusivly free rooms

    free_rooms(sat->rooms);
    free(sat->name);
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

void room_effects(_player *player)
{
    int level;
    int q;
    int b;
    char *input = NULL;
    int removal[3];
    bool isnulls[MAX_CARDS];
    switch (player->room->type->id)
    {
        case 0:
            if (!player->sat->plotted)
            {
                printf("Searching for satalites\n");
                player->sat->next = sat_gen(player, player->sat->level + 1, player->sat->seed + 1, player->sat->room_types);
                printf("Found course to satalite %s\n", player->sat->next->name);
                player->sat->plotted = true;
                printf("Head to the escape pods to go to %s\n", player->sat->next->name);
            }
            else
            {
                printf("Course already plotted\n");
            }
            break;
        case 2:
            if (player->sat->plotted)
            {
				printf("The escape pods are ready to leave, are you?\ny or n>");
                char c[10];
                fgets(c, 10, stdin);
				// printf("%c\n", c[0]);
                switch (c[0])
				{
					case 'y':
						printf("Leaving for %s\n", player->sat->next->name);
						satalite *temp = player->sat->next;
						free_sat(player->sat);
						player->sat = temp;
						printf("Entering %s\n", player->sat->name);
						player->room = player->sat->starting_room;
						player->room->seen = true;
						// printf("As you leave your pod you enter %s\n", player->room->type->descriptions[0]);
						return;
				}
            }
            printf("The escape pods are turned off\n");
            break;
        case 4:
            if (!player->room->used)
            {
                printf("Choose one\n1: Heal\n2: Get card\n3: Remove card\n>");
                char c[10];
                fgets(c, 10, stdin);
                switch (c[0])
                {
                    case '1':
                        player->health += 5;
                        player->health = fmin(player->health, player->maxHealth);
                        player->room->used = true;
                        break;
                    case '2':
                        level = (int)!(rand() % 10) + (int)!(rand() % 10) + (int)!(rand() % 10);
                        Card *card = random_card();
                        while (card->rarity != level)
                            card = random_card();
                        for (int i = 0; i < MAX_CARDS; i++)
                            if (player->deck[i] == NULL)
                            {
                                player->deck[i] = card;
                                printf("Gained: %s\n", card->name);
                                player->room->used = true;
                                return;
                            }
                        printf("Your ineventory is full\n");
                        room_effects(player);
                        break;
                    case '3':
                        input = malloc(20);
                        shuffle(player->deck);
                        for
                        printf("Which card would you like to remove from your deck?\n1. %s\n2. %s\n3. %s\n", removal[0]->name, removal[1]->name, removal[2]->name);
                        fgets(input, 15, stdin);
    	                for (b = 0; input[b] != '\n'; b++);
    	                input[b] = '\0';
    	                for (int i = 0; i < 3; i++)
    	                {
    	                    if (atoi(input) == i + 1)
    	                    {
    	                        switch (i)
    	                        {
    	                            case 2:
    	                                player->deck[q - 2] = player->deck[q - 1];
    	                            case 1:
    	                                player->deck[q - 1] = player->deck[q];
    	                            case 0:
    	                                player->deck[q] = NULL;
    	                                break;
    	                        }
    	                    }
    	                }
                        break;
                }
            }
            else
            {
                printf("The lab is out of power\n");
            }

            printf(" ");
            break;
    }
}