#ifdef __APPLE_CC__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "loadMap.h"
#include "drawMap.h"
#include "hedgehog.h"



void moveLeft(char map[][NbCol])		//la fonction va vérifier si on peut se déplacer vers la gauche et le faire le cas échéant
{
	int Mx = 0, My = 0;

	Mx = Player->x-1;
	My = Player->y;
	
	if (map[Mx][My]!='#')
	{
		Player->x = Mx;
        Player->angle = 0;
	}
}

void moveRight(char map[][NbCol])		//la fonction va vérifier si on peut se déplacer vers la droite et le faire le cas échéant
{
	int Mx = 0, My = 0;

	Mx = Player->x+1;
	My = Player->y;

	if (map[Mx][My]!='#')
	{
		Player->x = Mx;
        Player->angle= 180;
	}
}

void moveUp(char map[][NbCol])
{
    int Mx = 0, My = 0;
      
	Mx = Player->x;
	My = Player->y-1;

	if (map[Mx][My]!='#')
	{
		Player->y = My;
        Player->angle = 90;
	}
}

void moveDown(char map[][NbCol])	//droite
{
    int Mx = 0, My = 0;
    
	Mx = Player->x;
	My = Player->y+1;

	if (map[Mx][My]!='#')
	{
		Player->y = My;
        Player->angle = 270;
	}
}