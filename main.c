#ifdef __APPLE_CC__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "loadMap.h"
#include "drawMap.h"
#include "hedgehog.h"
#include "game.h"
#include "mobile.h"
#include "texture.h"
#include "missile.h"

char map[NbLin][NbCol];
player Player[1];
missile Missile[1];
struct mobileChain *mob;
struct mobileChain *wood;
struct mobileChain *mobShoot;
struct mobileChain *playerShoot;
struct mobileChain *bonusFruits;



void initRendering()
{
	glEnable(GL_DEPTH_TEST);
}

void handleResize(int width, int heigth)
{
	glViewport(0, 0, width, heigth);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double) NbCol,(double) NbLin, 0.0);
}

void Display()
{
        // Définit la couleur de fond 
	glClearColor(0.0, 0.0, 0.0, 1.0);
        // Efface l'écran 
	glClear(GL_COLOR_BUFFER_BIT);
        // Définit la matrice de modélisation active
        glMatrixMode(GL_MODELVIEW);
       
        //Instructions d'affichage//
        game(map);
        // Achève l'affichage
	glFlush();
}

int main(int argc, char *argv[])
{
   loadMap(map);//Charge la carte
   /* Gestion de graphique */
   glutInit(&argc, argv);
   // Position de la fenêtre 
   glutInitWindowPosition(800, 400);
   // Taille de la fenêtre  
   glutInitWindowSize(1000, 800);
   // Mode d'affichage 
   glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE); 
   // Création de la fenêtre 
   glutCreateWindow("S.O.S. Animaux");
   initRendering();
   // Fonction d'affichage 
   glutDisplayFunc(Display);
   // Fonction de redimentionnement 
   glutReshapeFunc(handleResize);
   // Fonction de glut gérant le clavier
   glutKeyboardFunc(Keyboard);
   // Fonction de gestion de la souris
   glutMouseFunc(mouse);
   // Fonction gérant le temps pour les transformations
   glutTimerFunc(1000/60, timer, 0);
   // Fonction gérant la lecture des textures
   initTextures();
   // Boucle principale
   glutMainLoop();
   return 0;	
}