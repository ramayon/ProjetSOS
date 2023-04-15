#ifndef _GAME_H_
#define _GAME_H_

#include <stdio.h>
#include <stdlib.h>

void testCollision();
void testCollisionPShoot();
void mobAutoShoot();
void removeDead(struct mobileChain *mob,struct mobileChain *thorn);
void mouse(int bouton,int etat,int x,int y);
void Keyboard(unsigned char key, int x, int y);
void game(char map[][NbCol]);

#endif