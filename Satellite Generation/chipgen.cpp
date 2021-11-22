#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int digcheck(char* input)
{
    //Checks if a string contains only numbers
    //Returns 0 on fail
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] < '0' || input[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}
//Speed
//Max Health
//Max Energy
//Max Ogygen
class chip{
    public:
        int stats[4] = {0, 0, 0, 0};

};
int main(int argc, char* argv[]){
    chip genchip;
    //Arg is Chip Level
    if (argc != 2 || digcheck(argv[1]) == 0)
    {
        printf("Invalid Arguments\n");
        return 1;
    }
    int level = atoi(argv[1]);

    if (level < 1)
    {
        printf("Chip Level must be a Natural Number\n");
        return 1;
    }
    srand(time(0));

    int powermod = (rand() % 50);

    if (powermod <= 50) {
        int numberofstats = rand() % 4;
        for (int i = 0; i < numberofstats; i++){
            genchip.stats[(rand() % 4)] += (((float)pow(level, 2)) / 10.0) * powermod / 10;
        }

    }
    printf("Powermod: %i\n", powermod);
    printf("Speed: %i\n", genchip.stats[0]);
    printf("Speed: %i\n", genchip.stats[1]);
    printf("Speed: %i\n", genchip.stats[2]);
    printf("Speed: %i\n", genchip.stats[3]);
}