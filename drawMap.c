#ifdef __APPLE_CC__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <FreeImage.h> // Include the FreeImage library

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "loadMap.h"
#include "drawMap.h"
#include "hedgehog.h"
#include "mobile.h"
#include "texture.h"

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void enterName()
{
	glPushMatrix();
	drawColoredSquare(6.9,1.3,13,1,0,0,0,0); // Dessine un rectangle noir pour le texte
	glColor3f(1.0f, 1.0f, 1.0f);
	drawBitmapText("Donne moi ton nom que je l'inscrive dans le livre", 7.0, 2);
	drawColoredSquare(1.1, 11.3, 5, 1, 0, 1, 1, 1); // Dessine un rectangle blanc
	glColor3f(0.0f, 0.0f, 0.0f);
	// Affiche un à un les caractères du nom introduit
	glRasterPos2f(1.2, 12);
	for (int i=0;i<inputNameIndex;i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Player->name[i]);
	}
}

void timer(int)
{
	if(PAUSE==false)
	{
		glutPostRedisplay();
		glutTimerFunc(2400/60, timer, 0);
		if(mob!=NULL)
		{
			struct mobile *temp=mob->first;
			while(temp!=NULL)
			{
				float *tp=&temp->translation;
				if(temp->alive==true)
				{
					if(temp->value==1||temp->value==2||temp->value==5||temp->value==6)
					{
						if(temp->translation>-25)
						{
							*tp-=temp->speed;
						}
						else if(temp->translation<-25)
						{
							*tp+=25;
						}
					}
					else if(temp->value==3||temp->value==4||temp->value==7||temp->value==8)
					{
						if(temp->translation<25)
						{
							*tp+=temp->speed;
						}
						else if(temp->translation>1)
						{
							*tp-=25;
						}
					}
				}
				temp=temp->next;
			}
		}
		if(mobShoot!=NULL)
		{
			struct mobile *temp3=mobShoot->first;
			while(temp3!=NULL)
			{
				float *tp3=&temp3->translation;
				if(temp3->shootAngle==(-1))
				{
					*tp3-=0.5;
				}
				else if(temp3->shootAngle==1)
				{
					*tp3+=0.5;
				}
				temp3=temp3->next;
			}
		}
		if(playerShoot!=NULL)
		{
			struct mobile *temp0=playerShoot->first;
			while(temp0!=NULL)
			{
				float *tp0=&temp0->translation;
				if(temp0->angle==0||temp0->angle==90)
				{
					*tp0-=0.3;
				}
				else if(temp0->angle==180||temp0->angle==270)
				{
					*tp0+=0.3;
				}
				temp0=temp0->next;
			}
		}
		if(wood!=NULL)
		{
			struct mobile *temp2=wood->first;
			while(temp2!=NULL)
			{
				float *tp=&temp2->translation;
				float *tp2=&temp2->x;

				if(temp2->value==1||temp2->value==3||temp2->value==5||temp2->value==7)
				{
					if((temp2->x+temp2->translation)>-5)
					{
						*tp-=temp2->speed;
					}
					else
					{
						*tp=0;
						*tp2=26;
					}
					if(Player->x>=temp2->x+temp2->translation&&Player->x<=temp2->x+5+temp2->translation&&Player->y==temp2->y) 
					{
						Player->x-=temp2->speed;
					}
				}
				else if(temp2->value==2||temp2->value==4||temp2->value==6||temp2->value==8)
				{
					if((temp2->x+temp2->translation)<26)
					{
						*tp+=temp2->speed;
					}
					else
					{
						*tp=0;
						*tp2=-5;
					}
					if(Player->x>=temp2->x+temp2->translation&&Player->x<=temp2->x+5+temp2->translation&&Player->y==temp2->y) 
					{
						Player->x+=temp2->speed;
					}
				}
			temp2=temp2->next;
			}
		}
	}
	else
	{
		glutTimerFunc(100, timer, 0); //Vérifie toutes les 100ms si la pause est terminée
	}
}

void drawShooting(struct mobile *temp, float translation)
{
    while (temp != NULL)
    {
        glPushMatrix();
        if (temp->angle == 0 || temp->angle == 180)
        {
            glTranslatef(temp->translation, 0.0, 0.0);
        }
        else if (temp->angle == 90 || temp->angle == 270)
        {
            glTranslatef(0.0, temp->translation, 0.0);
        }

        glBegin(GL_LINES);
        glColor3d(0.0, 0.0, 0.0); // Couleur des tirs noir

        if (temp->angle == 0)
        {
            glVertex2f(temp->x + 0.5, temp->y + 0.5);
            glVertex2f(temp->x + 0.2, temp->y + 0.5);
        }
        else if (temp->angle == 90)
        {
            glVertex2f(temp->x + 0.5, temp->y + 0.5);
            glVertex2f(temp->x + 0.5, temp->y + 0.2);
        }
        else if (temp->angle == 180)
        {
            glVertex2f(temp->x + 0.5, temp->y + 0.5);
            glVertex2f(temp->x + 0.8, temp->y + 0.5);
        }
        else if (temp->angle == 270)
        {
            glVertex2f(temp->x + 0.5, temp->y + 0.5);
            glVertex2f(temp->x + 0.5, temp->y + 0.8);
        }

        glEnd();
        glPopMatrix();

        temp = temp->next;
    }
}

void drawObject(float x, float y, int tile, float sizeX, float sizeY, float translation)
{
	glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureIds[tile]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTranslatef(translation, 0.0, 0.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); // sommet en bas à gauche
        glVertex2f(x, y);
        glTexCoord2f(0.0f, 0.0f); // sommet en haut à gauche
        glVertex2f(x, y + sizeY);
        glTexCoord2f(1.0f, 0.0f); // sommet en haut à droite
        glVertex2f(x + sizeX, y + sizeY);
        glTexCoord2f(1.0f, 1.0f); // sommet en bas à droite
        glVertex2f(x + sizeX, y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();
}

void drawColoredSquare(float x, float y, float sizeX, float sizeY, float translation, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(translation, 0.0, 0.0);
    glBegin(GL_QUADS);
        glColor3f(r, g, b);
        glVertex2f(x, y);
        glVertex2f(x, y + sizeY);
        glVertex2f(x + sizeX, y + sizeY);
        glVertex2f(x + sizeX, y);
    glEnd();
    glPopMatrix();
}

void drawBitmapText(char *string,float x,float y) 
{  
	char *c;
	glRasterPos2f(x,y);
	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void drawMap(char map[][NbCol])			// fonction qui affiche TOUT
{
	if(menu==0)
	{
		drawObject(0,0,12,26,26,0); // Affiche le fond du menu 0.
		drawColoredSquare(6,8.5,6,1, 0,0,0.5,0.5); // Affiche le bouton "Nouvelle partie".
		drawColoredSquare(6,10.5,6,1, 0,0,0.5,0.5); // Affiche le bouton "Parametres".
		drawColoredSquare(6,12.5,6,1, 0,0,0.5,0.5); // Affiche le bouton "Touches de jeu".
		drawColoredSquare(6,14.5,6,1, 0,0,0.5,0.5); // Affiche le bouton "Quitter".
		glColor3f(0.0, 0.0, 0.0);
			drawBitmapText("Nouvelle partie", 7.2, 9.2);
			drawBitmapText("Parametres", 7.6,11.2);
			drawBitmapText("Touches de jeu", 7.2, 13.2);
			drawBitmapText("Quitter", 8.0, 15.2);
			glEnd();
		glFlush();
	}
	else if(menu==1)
	{
		if(Player->vie>0)
		{
			int i, j;
			// Commence l'affichage
			// Bind the texture
			glEnable(GL_TEXTURE_2D);
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est un mur, on dessine un carré 
				if(map[i][j] == '0'||map[i][j] == '4'||map[i][j] == '5')
				{   
					drawObject(i,j,0,1,1,0);
				}
				// Si c'est de l'eau, on dessine un carré 
				else if(map[i][j] == '3')
				{   
					drawObject(i,j,1,1,1,0);
				}
				// Si c'est de l'herbe, on dessine un carré
				else if(map[i][j] == '2'||map[i][j] == '6')
				{   
					drawObject(i,j,2,1,1,0);
				}
			glDisable(GL_TEXTURE_2D);
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est une ligne, on dessine une ligne
				if(map[i][j] == '4')
				{   
					drawColoredSquare(i+0.5,j+0.25,0.75,0.5,0,0.7,0.7,0.0); // Couleur jaune des traits de route
				}
				else if(map[i][j] == '5')
				{   
					drawColoredSquare(i,j+0.4,1,0.2,0,1.0,1.0,1.0); // Couleur blanche des traits de route
				}
				else if(map[i][j] == '#')
				{   
					drawColoredSquare(i,j,1,1,0,0.5,0.5,0.05); // Couleur brune des bords de map
				}
			// Achève l'affichage
			struct mobile *temp2=wood->first;
			while (temp2!=NULL)
			{
				drawObject(temp2->x,temp2->y,3,5,1,temp2->translation);
				temp2=temp2->next;
			}
			if(Player->alive==false)
			{
				// Applique la texture de mort sur la joueur
				drawObject(Player->x,Player->y,16,1,1,0);
			}
			else
			{
				if(Player->angle==0) //Gauche
				{
					drawObject(Player->x,Player->y,4,1,1,0);
				}
				else if(Player->angle==90) //Haut
				{
					drawObject(Player->x,Player->y,5,1,1,0);
				}
				else if(Player->angle==270) //Bas
				{
					drawObject(Player->x,Player->y,6,1,1,0);
				}
				else if(Player->angle==180) //Droite
				{
					drawObject(Player->x,Player->y,7,1,1,0);
				}
			}
			//affichage mobile
			struct mobile *temp=mob->first;
			while(temp!=NULL)
			{
				if(temp->alive==true&&(temp->value==2||temp->value==6))
				{
					drawObject(temp->x,temp->y,9,1,1,temp->translation);
				}
				else if(temp->alive==true&&(temp->value==1||temp->value==5))
				{
					drawObject(temp->x,temp->y,8,1,1,temp->translation);
				}
				else if(temp->alive==true&&(temp->value==3||temp->value==7))
				{
					drawObject(temp->x,temp->y,11,1,1,temp->translation);
				}
				else if(temp->alive==true&&(temp->value==4||temp->value==8))
				{
					drawObject(temp->x,temp->y,10,1,1,temp->translation);
				}
				if(temp->shoot == true)
				{
					if(mobShoot->size>0)
					{
						//affichage mobShoot
						drawShooting(mobShoot->first, 0);
					}
				}
				temp=temp->next;
			}
			//affichage playerShoot
			drawShooting(playerShoot->first, 0);
			glColor3f(0.0, 0.0, 0.0);
			char score[10];
			char vie[10];
			char minutesStr[3];
			char secondsStr[3];
			tostring(score,Player[0].score);
			tostring(vie,Player[0].vie);
			tostring(minutesStr,timeGame/60);
			tostring(secondsStr,timeGame%60);
			drawBitmapText("Score: ", 2.0, 0.8);
			drawBitmapText(score, 4.0, 0.8);
			drawBitmapText("Vie: ", 5.5, 0.8);
			drawBitmapText(vie, 7.0, 0.8);
			drawBitmapText("Timer: ", 9.0, 0.8);
			drawBitmapText(minutesStr, 11.2, 0.8);
			drawBitmapText(":", 11.5, 0.8);
			drawBitmapText(secondsStr, 11.8, 0.8);
			glEnd();
		glFlush();
		glutPostRedisplay();
		}
		else if(Player->vie==0)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glColor3d(1.0, 0.0, 1.0);
			drawBitmapText("GAME OVER", 11.0, 14.0);
		}
	}
	else if(menu==2)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		drawObject(0,0,13,26,26,0); // Affiche le fond du menu 2
		drawColoredSquare(19.5,8,6.3,2,0,0.5,0.5,0.5); // Couleur grise de la case montrant le niveau de difficulté actuel.
		drawColoredSquare(2,23,7,1,0,0.0,0.5,0.5); // Couleur bleu clair de la case retour au menu principal.
		glColor3f(0.0, 0.0, 0.0);
			drawBitmapText("Difficulté actuelle :", 19.7, 8.7);
			glColor3d(0.0, 1.0, 0.2);
			drawBitmapText("Facile (vit. x1)", 19.7,11.7);
			if(difficulty==1)
			{
				drawBitmapText("Facile (vit. x1)", 19.7,9.7);
			}
			glColor3d(0.0, 0.2, 1.0);
			drawBitmapText("Normal (vit. x2)", 19.7, 13.7);
			if(difficulty==2)
			{
				drawBitmapText("Normal (vit. x2)", 19.7,9.7);
			}
			glColor3d(1.0, 0.0, 0.2);
			drawBitmapText("Difficile (vit. & voit. x2)", 19.7, 15.7);
			if(difficulty==3)
			{
				drawBitmapText("Difficile (vit. & voit. x2)", 19.7,9.7);
			}
			glColor3d(0.0, 0.0, 0.0);
			drawBitmapText("Retour au menu principal",2.3,23.7);
			glEnd();
		glFlush();
		glutPostRedisplay();
	}
	else if(menu==3) //menu touche du jeu
	{
		glClear(GL_COLOR_BUFFER_BIT);
		drawObject(0,0,14,26,26,0); // Affiche le fond du menu 3
		drawColoredSquare(2,23,7,1,0,0.0,0.5,0.5); // Couleur bleu clair de la case retour au menu principal.
		glColor3f(0.0, 0.0, 0.0);
			drawBitmapText("Z = Avancer", 18.5, 12.2);
			drawBitmapText("S = Reculer", 18.5, 13.2);
			drawBitmapText("Q = Gauche", 18.5, 14.2);
			drawBitmapText("D = Droite", 18.5, 15.2);
			drawBitmapText("A = Tir", 18.5, 16.2);
			drawBitmapText("E = Tir Electrique", 18.5, 17.2);
			drawBitmapText("Retour au menu principal",2.3,23.7);
			glEnd();
		glFlush();
		glutPostRedisplay();
	}
	else if(inputName==true)
	{
		drawObject(0,0,15,26,26,0); // Affiche le fond du menu 4
		enterName(); //Affiche le pop-up pour entrer le nom
	}
	glFlush();
	glutPostRedisplay();
}

//faire correspondre à la quantité de playerShoot en jeu ET activer la fonction de dessin tant que playerShoot->first!=NULL