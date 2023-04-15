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

void timer(int)
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
			}
		temp2=temp2->next;
		}
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

void drawObject(int x, int y, int tile, int size, float translation)
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
        glVertex2f(x, y + 1);
        glTexCoord2f(1.0f, 0.0f); // sommet en haut à droite
        glVertex2f(x + size, y + 1);
        glTexCoord2f(1.0f, 1.0f); // sommet en bas à droite
        glVertex2f(x + size, y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
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
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_QUADS);
			glColor3d(0.5, 0.5, 0.5); // Couleur gris
				// Place les points du carré
				// Specify the texture coordinates for each vertex
				glVertex2d(0, 0);
				glVertex2d(0, 26); 
				glVertex2d(26, 26); 
				glVertex2d(26,0);
		glEnd();
		glBegin(GL_QUADS);
		glColor3d(0.0, 0.5, 0.5); // Couleur bleu clair
				// Place les points du carré 
				glVertex2d(10, 9); 
				glVertex2d(10, 10); 
				glVertex2d(16, 10); 
				glVertex2d(16,9);
		glBegin(GL_QUADS);
		glColor3d(0.0, 0.5, 0.5); // Couleur bleu clair
				// Place les points du carré 
				glVertex2d(10, 11); 
				glVertex2d(10, 12); 
				glVertex2d(16, 12); 
				glVertex2d(16,11);
		glBegin(GL_QUADS);
		glColor3d(0.0, 0.5, 0.5); // Couleur bleu clair
				// Place les points du carré 
				glVertex2d(10, 13); 
				glVertex2d(10, 14); 
				glVertex2d(16, 14); 
				glVertex2d(16,13);
		glBegin(GL_QUADS);
		glColor3d(0.0, 0.5, 0.5); // Couleur bleu clair
				// Place les points du carré 
				glVertex2d(10, 15); 
				glVertex2d(10, 16); 
				glVertex2d(16, 16); 
				glVertex2d(16,15);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
			drawBitmapText("Nouvelle partie", 11.2, 9.7);
			drawBitmapText("Parametres", 11.6,11.7);
			drawBitmapText("Touches de jeu", 11.2, 13.7);
			drawBitmapText("Quitter", 12.0, 15.7);
			glEnd();
		glFlush();
	}
	else if(menu==1)
	{
		if(Player->alive==true)
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
					drawObject(i,j,0,1,0);
				}
				// Si c'est de l'eau, on dessine un carré 
				else if(map[i][j] == '3')
				{   
					drawObject(i,j,1,1,0);
				}
				// Si c'est de l'herbe, on dessine un carré
				else if(map[i][j] == '2'||map[i][j] == '6')
				{   
					drawObject(i,j,2,1,0);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est une ligne, on dessine une ligne
				if(map[i][j] == '4')
				{   
					glColor3d(0.7, 0.7, 0.0); // Couleur jaune
					// Place les points du carré 
					glVertex2f(i+0.5, j+0.25); 
					glVertex2f(i+0.5, j+0.75); 
					glVertex2f(i+1.25, j+0.75); 
					glVertex2f(i+1.25, j+0.25);
				}
				else if(map[i][j] == '5')
				{   
					glColor3d(1.0, 1.0, 1.0); // Couleur blanche
					// Place les points du carré 
					glVertex2f(i, j+0.4); 
					glVertex2f(i, j+0.6); 
					glVertex2f(i+1, j+0.6); 
					glVertex2f(i+1, j+0.4);
				}
				else if(map[i][j] == '#')
				{   
					glColor3f(0.5, 0.5, 0.05); // Couleur brune des bords
					// Place les points du carré 
					glVertex2d(i, j); 
					glVertex2d(i, j+1); 
					glVertex2d(i+1, j+1); 
					glVertex2d(i+1, j);
				}
			// Achève l'affichage
			glEnd();
			struct mobile *temp2=wood->first;
			while (temp2!=NULL)
			{
				drawObject(temp2->x,temp2->y,3,5,temp2->translation);
				temp2=temp2->next;
			}
			if(Player->angle==0) //Gauche
			{
				drawObject(Player->x,Player->y,4,1,0);
			}
			else if(Player->angle==90) //Haut
			{
				drawObject(Player->x,Player->y,5,1,0);
			}
			else if(Player->angle==270) //Bas
			{
				drawObject(Player->x,Player->y,6,1,0);
			}
			else if(Player->angle==180) //Droite
			{
				drawObject(Player->x,Player->y,7,1,0);
			}
			glEnd();
			//affichage mobile
			struct mobile *temp=mob->first;
			while(temp!=NULL)
			{
				if(temp->alive==true&&(temp->value==2||temp->value==6))
				{
					drawObject(temp->x,temp->y,9,1,temp->translation);
				}
				else if(temp->alive==true&&(temp->value==1||temp->value==5))
				{
					drawObject(temp->x,temp->y,8,1,temp->translation);
				}
				else if(temp->alive==true&&(temp->value==3||temp->value==7))
				{
					drawObject(temp->x,temp->y,11,1,temp->translation);
				}
				else if(temp->alive==true&&(temp->value==4||temp->value==8))
				{
					drawObject(temp->x,temp->y,10,1,temp->translation);
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
			tostring(score,Player[0].score);
			tostring(vie,Player[0].vie);
			//tostring(timer,player->timer);
			drawBitmapText("Score: ", 2.0, 0.8);
			drawBitmapText(score, 4.0, 0.8);
			drawBitmapText("Vie: ", 5.5, 0.8);
			drawBitmapText(vie, 7.0, 0.8);
			drawBitmapText("Timer: ", 9.0, 0.8);
			glEnd();
		glFlush();
		glutPostRedisplay();
		}
		else if(Player->alive==false)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glColor3d(1.0, 0.0, 1.0);
			drawBitmapText("GAME OVER", 11.0, 14.0);
		}
	}
	else if(menu==2)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_QUADS);
			glColor3d(0.5, 0.5, 0.5); // Couleur gris
				// Place les points du carré 
				glVertex2d(0, 0); 
				glVertex2d(0, 26); 
				glVertex2d(26, 26); 
				glVertex2d(26,0);
		glEnd();
		glBegin(GL_QUADS);
		glColor3d(0.5,0.5,0.0); // Couleur brun clair
				// Place les points du carré 
				glVertex2d(5, 9); 
				glVertex2d(5, 16); 
				glVertex2d(12, 16); 
				glVertex2d(12,9);
		glEnd();
		glBegin(GL_QUADS);
		glColor3d(0.0, 0.5, 0.5); // Couleur bleu clair
				// Place les points du carré 
				glVertex2d(2, 23); 
				glVertex2d(2, 24); 
				glVertex2d(10, 24); 
				glVertex2d(10,23);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
			drawBitmapText("Niveau de difficulte :", 6, 9.7);
			glColor3d(0.0, 1.0, 0.2);
			drawBitmapText("Facile", 6,11.7);
			glColor3d(0.0, 0.2, 1.0);
			drawBitmapText("Normal", 6, 13.7);
			glColor3d(1.0, 0.0, 0.2);
			drawBitmapText("Difficile", 6, 15.7);
			glColor3d(0.0, 0.0, 0.0);
			drawBitmapText("Retour au menu principal",2.3,23.7);
			glEnd();
		glFlush();
		glutPostRedisplay();
	}
	else if(menu==3) //menu touche du jeu
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_QUADS);
			glColor3d(0.5, 0.5, 0.5); // Couleur gris
				// Place les points du carré 
				glVertex2d(0, 0); 
				glVertex2d(0, 26); 
				glVertex2d(26, 26); 
				glVertex2d(26,0);
		glEnd();
		glBegin(GL_QUADS);
		glColor3d(0.0, 0.5, 0.5); // Couleur bleu clair
				// Place les points du carré 
				glVertex2d(2, 23); 
				glVertex2d(2, 24); 
				glVertex2d(10, 24); 
				glVertex2d(10,23);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
			drawBitmapText("Z pour avancer", 6, 9.7);
			drawBitmapText("S pour reculer", 6,11.7);
			drawBitmapText("Q pour aller a gauche", 6, 13.7);
			drawBitmapText("D pour aller a droite", 6, 15.7);
			drawBitmapText("A pour tirer des epines", 6, 17.7);
			drawBitmapText("E pour tirer des epines electrique qui immobilisent", 6, 19.7);
			drawBitmapText("Retour au menu principal",2.3,23.7);
			glEnd();
		glFlush();
		glutPostRedisplay();
	}
	else if(menu==4)
	{

	}
	glFlush();
	glutPostRedisplay();
}

//faire correspondre à la quantité de playerShoot en jeu ET activer la fonction de dessin tant que playerShoot->first!=NULL