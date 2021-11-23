#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "Sat_gen_c/sat_gen.h"

int main(int argc, char *argv[])
{
    rewind(stdout);
    if (argc != 3) // check if we have the right amount of aruments
    {
        printf("please have a argument\n");
        return 0;
    }
    printf("Creating satalite\n");
    struct satalite *sat = sat_gen(atoi(argv[1]), atoi(argv[2]));
    printf("Printing satalite\n");
    draw_map(sat);
    struct room *player_room = sat->starting_room;
    player_room->seen = true;
    char input[50];
    int i;
    while (true)
    {
        printf(">");
        fgets(input, 50, stdin);
        printf("\033[2J\033[1;1H");
        for (i = 0; input[i] != '\n'; input[i] = tolower(input[i]), i++);
        input[i] = '\0';
        if (strcmp(input, "door\0") == 0 || strcmp(input, "doors\0") == 0)
        {
            for (struct door *current = player_room->doors; current != NULL; current = current->next)
            {
                printf("%i %c\n", current->doorp->data, current->direction);
            }
        }
        else if (!strcmp(input, "quit\0"))
        {
            break;
        }
        else if (!strcmp(input, "map\0"))
        {
            draw_map(sat);
        }

    }
    printf("Freeing things\n");
    free_sat(sat);
    printf("Things have been freed\n");
}