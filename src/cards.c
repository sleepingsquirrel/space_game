#include <stdio.h>

//https://www.geeksforgeeks.org/function-pointer-in-c/

const int EFFECT_COUNT;
const int MAX_CARDS;

typedef struct _Card {
    //Card name
    string name;
    //Cost in oxygen, energy, health
    int cost_o;
    int cost_e;
    int cost_h;
    //amount of targets
    int targets;
    //damage in oxygen, energy, health
    int dam_o;
    int dam_e;
    int dam_h;
    //list of which effects this card does and does not have
    bool effects[EFFECT_COUNT];
    Card *next;
} Card;

void 




