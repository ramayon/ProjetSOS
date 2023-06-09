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
	
	if (map[Mx][My]!='#'&&Mx>=0)
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

	if (map[Mx][My]!='#'&&Mx<=25)
	{
		Player->x = Mx;
        Player->angle= 180;
	}
}

void moveUp(char map[][NbCol])		//la fonction va vérifier si on peut se déplacer vers le haut et le faire le cas échéant
{
    int Mx = 0, My = 0;
      
	Mx = Player->x;
	My = Player->y-1;

	if (map[Mx][My]!='#'&&My>=0)
	{
		Player->y = My;
        Player->angle = 90;
	}
}

void moveDown(char map[][NbCol])	//la fonction va vérifier si on peut se déplacer vers le bas et le faire le cas échéant
{
    int Mx = 0, My = 0;
    
	Mx = Player->x;
	My = Player->y+1;

	if (map[Mx][My]!='#'&&My<=25)
	{
		Player->y = My;
        Player->angle = 270;
	}
}