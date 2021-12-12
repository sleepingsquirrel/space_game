#include <stdio.h>

room_type *load_room_types(const char *filename)
{
    //Opens the given file
    FILE *file = fopen(filename, "r");
    //Exits function if file does not exist
    if (file == NULL)
    {
        return 0;
    }
    //Initializes all remaining variables to NULL
    room_type *start = NULL;
    room_type *current;
    char buffer;
    char words[9][300];

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j <  300; j++)
        {
            words[i][j] = '\0';
        }
    }
    //TODO: switch to fread
    int j,k,l;
    j = 0;
    k = 0;
    l = 0;
    while (fread(&buffer, sizeof(char), 1, file))
    {
        if (l < 3)
            k++;
            if (buffer == '\n' || buffer == ',')
            {
                words[j][k - 1] = '\0';
                k = 0;
                j++;
                if (buffer != ',')
                {
                    l++;
                    if(!fread(&buffer, sizeof(char), 1, file))
                    {
                        break;
                    };
                }
            }
            else
            {
                words[j][k - 1] = buffer;
            }
        }
        else
        {
            current = malloc(sizeof(room_type));
            current->name = malloc(strlen(words[0]) + 1);
            strcpy(current->name, &words[0][0]);
            current->name[strlen(words[0])] = '\0';
            current->id = atoi(words[1]);
            current->descriptions = malloc(sizeof(char*) * j - 1);
            int i;
            for (i = 0; i < j; i++)
            {
                current->descriptions[i] = malloc(strlen(words[i + 2]) + 1)
                for (int c = 0; c < strlen(words[i + 2]) + 1; c++)
                {
                   current->descriptions[i] = words[i + 2][c];
                }
                current->descriptions[i] = '\0'
            }

            //Places current at the front of the linked list
            current->next = start;
            start = current;
            j = 0;
            l = 0;
        }
    }
    //Frees buffer and file
    fclose(file);
    //Returns the start of the linked list.
    //printf("%s\n", start->name);
    return start;
}

void free_room_types(room_type *type)
{
    if (type->next != NULL)
    {
        free_room_types(type->next);
    }
    for(int i = 0; type->descriptions[i]; i++)
    {
        free(type->descriptions[i]);
    }
    free(type->descriptions);
    free(type->name);
}