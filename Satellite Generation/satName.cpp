#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <time.h>

using namespace std;
string satname(void);

int main(void)
{
    string name = satname();
    cout << name;
}

string satname(void)
{
    srand(time(0));
    string firstWord[] = {"CUSNASA-" /*canusa*/, "FRSRP-"/*france*/, "NORSP-"/*nordic*/, "RNC&S-"/*russia*/, "CUSP-" /*china*/, "UNISEP-" /*un*/, "SASP-" /*south america*/, "OCEANS-" /*Ausnz*/,"Unknown-" /*unknown*/};
    int word1num = rand() % 10 + 1;
    int numnum = rand() % 9 + 1;
    int letternum = rand() % 26 + 1;
    string fullname;

    fullname = firstWord[word1num];
    if (fullname == "Unknown-")
    {
        fullname += "??";
    }
    else
    {
        fullname += letternum + 65;
        fullname += numnum + 48;
    }

    return fullname;
}