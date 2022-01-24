#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*int main()
{
    char *str;
    for (int i = 0; i < 100000000; i++)
    {
        str = sat_name();
        printf("%s\n", str);
        free(str);
    }
}*/
char *firstWord[] = {"GNASA" /*canusa*/, "GSSA"/*russia*/, "DASC" /*south america*/, "UNSP" ,"Unknown" /*unknown*/};

char *sat_name(int faction)
{

    int numnum = rand() % 9;
    int letternum = rand() % 26;
    char *str = malloc(sizeof(char) * (strlen(firstWord[faction]) + 4));
    int i;
    for (i = 0; i < strlen(firstWord[faction]); ++i)
    {
        str[i] = firstWord[faction][i];
    }
    str[i++] = '-';
    if (faction == 8)
    {
        str[i++] = '?';
        str[i++] = '?';
    }
    else
    {
        str[i++] = letternum + 'A';
        str[i++] = numnum + '0';
    }
    str[i++] = '\0';
    return str;
}
