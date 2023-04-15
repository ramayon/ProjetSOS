#ifndef _LOADMAP_H_
#define _LOADMAP_H_

#include <stdbool.h>

#define NbLin 26
#define NbCol 26

typedef struct play
{
	int vie;
	float x; //position en X
    float y; //position en Y
    float angle;
    int score;
    bool alive; //si le joueur est en vie ou non
}player;

extern player Player[1];
extern struct mobileChain *mob;
extern struct mobileChain *wood;
extern struct mobileChain *mobShoot;
extern struct mobileChain *playerShoot;
extern unsigned int menu;
extern bool FIRE;
bool loadMap(char map[][NbCol]);

#endif
