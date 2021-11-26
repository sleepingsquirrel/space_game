#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
char *sat_name();

int main()
{
    char *str;
    for (int i = 0; i < 100000000; i++)
    {
        str = sat_name();
        printf("%s\n", str);
        free(str);
    }
}

char *sat_name()
{

    char *firstWord[] = {"CUSNASA" /*canusa*/, "FRSRP"/*france*/, "NORSP"/*nordic*/, "RNC&S"/*russia*/, "CUSP" /*china*/, "UNISEP" /*un*/, "SASP" /*south america*/, "OCEANS" /*Ausnz*/,"Unknown" /*unknown*/};
    int word1num = rand() % 9;
    int numnum = rand() % 9;
    int letternum = rand() % 26;
    char *str = malloc(sizeof(char) * (strlen(firstWord[word1num]) + 4));
    int i;
    for (i = 0; i < strlen(firstWord[word1num]); ++i)
    {
        str[i] = firstWord[word1num][i];
    }
    str[i++] = '-';
    if (word1num == 8)
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
