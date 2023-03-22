#ifndef _LOADMAP_H_
#define _LOADMAP_H_

#include <stdbool.h>

#define NbLin 26
#define NbCol 26

typedef struct play
{
	int vie;
	int x; //position en X
    int y; //position en Y
    float angle;
    int score;
    bool alive; //si le joueur est en vie ou non
}player;

typedef struct thorn
{
    int x; //position en X
    int y; //position en Y
    float speed;
    float angle;
    bool alive; //si le thorn est actif ou non
}thorn;

extern player Player[1];
extern thorn Thorn[5];
extern struct mobileChain *mob;
extern struct mobileChain *wood;
extern unsigned int menu;
extern bool FIRE;
extern unsigned int countThorn;
bool loadMap(char map[][NbCol]);

#endif
