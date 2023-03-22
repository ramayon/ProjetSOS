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
	int i;
	glutPostRedisplay();
	glutTimerFunc(2400/60, timer, 0);
	for(i=0;i<5;i++)
	{
		if(Thorn[i].alive==true)
		{
			if(Thorn[i].angle==0||Thorn[i].angle==90)
			{
				Thorn[i].speed-=0.2;
			}
			else if(Thorn[i].angle==180||Thorn[i].angle==270)
			{
				Thorn[i].speed+=0.2;
			}
		}
	}
	struct mobile *temp=mob->first;
	while(temp!=NULL)
	{
		float *tp=&temp->translation;
		if(temp->alive==true)
		{
			if(temp->value==2||temp->value==6)
			{
				if(temp->translation>-23)
				{
					*tp-=0.3;
				}
				else if(temp->translation<-23)
				{
					*tp+=23;
				}
			}
			else if(temp->value==1||temp->value==5)
			{
				if(temp->translation>-23)
				{
					*tp-=0.5;
				}
				else if(temp->translation<=-23)
				{
					*tp+=23;
				}
			}
			else if(temp->value==3||temp->value==7)
			{
				if(temp->translation<23)
				{
					*tp+=0.3;
				}
				else if(temp->translation>1)
				{
					*tp-=23;
				}
			}
			else if(temp->value==4||temp->value==8)
			{
				if(temp->translation<23)
				{
					*tp+=0.5;
				}
				else if(temp->translation>1)
				{
					*tp-=23;
				}
			}
		}
		temp=temp->next;
	}
	struct mobile *temp2=wood->first;
	while(temp2!=NULL)
	{
		float *tp=&temp2->translation;
		float *tp2=&temp2->x;

		if(temp2->value<=5||(temp2->value>10&&temp2->value<=15))
		{
			if((temp2->x+temp2->translation)>1)
			{
				*tp-=0.1;
			}
			else
			{
				*tp=0;
				*tp2=24;
			}
		}
		else if((temp2->value>5&&temp2->value<=10)||(temp2->value>15&&temp2->value<=20))
		{
			if((temp2->x+temp2->translation)<24)
			{
				*tp+=0.1;
			}
			else
			{
				*tp=0;
				*tp2=1;
			}
		}
		if((temp2->value>20&&temp2->value<=25)||(temp2->value>30&&temp2->value<=35))
		{
			if((temp2->x+temp2->translation)>1)
			{
				*tp-=0.1;
			}
			else
			{
				*tp=0;
				*tp2=24;
			}
		}
		else if((temp2->value>25&&temp2->value<=30)||(temp2->value>35&&temp2->value<=40))
		{
			if((temp2->x+temp2->translation)<24)
			{
				*tp+=0.1;
			}
			else
			{
				*tp=0;
				*tp2=1;
			}
		}
	temp2=temp2->next;
	}
	/*switch(stateR)
	{
		case 1:
			if(x_fromR_position>-23)
			{
				x_fromR_position-=0.2;
			}
			else
			{
				stateR=2;
			}
			break;
		case 2:
			if(x_fromR_position<-23)
			{
				x_fromR_position+=23;
			}
			else
			{
				stateR=1;
			}
			break;
	}
	switch(stateL)
	{
		case 1:
			if(x_fromL_position<23)
			{
				x_fromL_position+=0.2;
			}
			else
			{
				stateL=2;
			}
			break;
		case 2:
			if(x_fromL_position>1)
			{
				x_fromL_position-=23;
			}
			else
			{
				stateL=1;
			}
			break;
	}*/
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
			glBindTexture(GL_TEXTURE_2D, textureIds[0]);
			glBegin(GL_QUADS);
			glColor3d(0.5, 0.5, 0.5); // Couleur grise
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est un mur, on dessine un carré 
				if(map[i][j] == '0'||map[i][j] == '4'||map[i][j] == '5')
				{   
					// Place les points du carré 
					glTexCoord2f(0.0f, 0.0f); // sommet en bas à gauche
					glVertex2d(i, j); 
					glTexCoord2f(0.0f, 1.0f); // sommet en haut à gauche
					glVertex2d(i, j+1);
					glTexCoord2f(1.0f, 1.0f); // sommet en haut à droite
					glVertex2d(i+1, j+1);
					glTexCoord2f(1.0f, 0.0f); // sommet en bas à droite
					glVertex2d(i+1, j);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureIds[1]);
			glBegin(GL_QUADS);
			glColor3d(0.4, 0.6, 0.6); // Couleur bleue
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est de l'eau, on dessine un carré 
				if(map[i][j] == '3')
				{   
					// Place les points du carré 
					glTexCoord2f(0.0f, 0.0f); // sommet en bas à gauche
					glVertex2d(i, j); 
					glTexCoord2f(0.0f, 1.0f); // sommet en haut à gauche
					glVertex2d(i, j+1);
					glTexCoord2f(1.0f, 1.0f); // sommet en haut à droite
					glVertex2d(i+1, j+1);
					glTexCoord2f(1.0f, 0.0f); // sommet en bas à droite
					glVertex2d(i+1, j);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureIds[2]);
			glBegin(GL_QUADS);
			glColor3d(0.2, 0.7, 0.2); // Couleur verte
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est de l'herbe, on dessine un carré 
				if(map[i][j] == '2'||map[i][j] == '6')
				{   
					// Place les points du carré 
					glTexCoord2f(0.0f, 0.0f); // sommet en bas à gauche
					glVertex2d(i, j); 
					glTexCoord2f(0.0f, 1.0f); // sommet en haut à gauche
					glVertex2d(i, j+1);
					glTexCoord2f(1.0f, 1.0f); // sommet en haut à droite
					glVertex2d(i+1, j+1);
					glTexCoord2f(1.0f, 0.0f); // sommet en bas à droite
					glVertex2d(i+1, j);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glColor3d(0.7, 0.7, 0.0); // Couleur jaune
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est une ligne, on dessine une ligne
				if(map[i][j] == '4')
				{   
					// Place les points du carré 
					glVertex2f(i, j+0.25); 
					glVertex2f(i, j+0.75); 
					glVertex2f(i+0.75, j+0.75); 
					glVertex2f(i+0.75, j+0.25);
				}
			glEnd();
			glBegin(GL_QUADS);
			glColor3d(1.0, 1.0, 1.0); // Couleur blanche
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est une ligne, on dessine une ligne
				if(map[i][j] == '5')
				{   
					// Place les points du carré 
					glVertex2f(i, j+0.4); 
					glVertex2f(i, j+0.6); 
					glVertex2f(i+1, j+0.6); 
					glVertex2f(i+1, j+0.4);
				}
			glEnd();
			glBegin(GL_QUADS);
			glColor3f(0.5, 0.5, 0.05); // Couleur brune des bords
			// Parcourt toutes les cellules de la matrice 
			for(i=0; i<NbCol; i++)
			for(j=0; j<NbLin; j++)
				// Si c'est une ligne, on dessine une ligne
				if(map[i][j] == '#')
				{   
					// Place les points du carré 
					glVertex2d(i, j); 
					glVertex2d(i, j+1); 
					glVertex2d(i+1, j+1); 
					glVertex2d(i+1, j);
				}
			// Achève l'affichage
			glEnd();
			struct mobile *temp2=wood->first;
			while(temp2!=NULL)
			{
				glPushMatrix();
				glEnable(GL_TEXTURE_2D);
			    glBindTexture(GL_TEXTURE_2D, textureIds[3]);
				glTranslatef(temp2->translation,0.0,0.0);
				glBegin(GL_QUADS);
					glColor3d(0.5, 0.35, 0.05); // Couleur marron
					glTexCoord2f(0.0f, 0.0f); // sommet en bas à gauche
					glVertex2f(temp2->x, temp2->y);
					glTexCoord2f(0.0f, 1.0f); // sommet en haut à gauche
					glVertex2f(temp2->x, temp2->y+1);
					glTexCoord2f(1.0f, 1.0f); // sommet en haut à droite
					glVertex2f(temp2->x+1, temp2->y+1);
					glTexCoord2f(1.0f, 0.0f); // sommet en bas à droite
					glVertex2f(temp2->x+1, temp2->y);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				glPopMatrix();
				temp2=temp2->next;
			}
			if(Player->angle==0) //Gauche
			{
				glBegin(GL_TRIANGLES);
				glColor3d(1.0, 0.0, 1.0); // Couleur violette
				glVertex2f(Player->x, Player->y+0.5); 
				glVertex2f(Player->x+1, Player->y+1); 
				glVertex2f(Player->x+1, Player->y);
			}
			else if(Player->angle==90) //Haut
			{
				glBegin(GL_TRIANGLES);
				glColor3d(1.0, 0.0, 1.0); // Couleur violette
				glVertex2f(Player->x, Player->y+1); 
				glVertex2f(Player->x+1, Player->y+1); 
				glVertex2f(Player->x+0.5, Player->y);
			}
			else if(Player->angle==270) //Bas
			{
				glBegin(GL_TRIANGLES);
				glColor3d(1.0, 0.0, 1.0); // Couleur violette
				glVertex2f(Player->x, Player->y); 
				glVertex2f(Player->x+1, Player->y); 
				glVertex2f(Player->x+0.5, Player->y+1);
			}
			else if(Player->angle==180) //Droite
			{
				glBegin(GL_TRIANGLES);
				glColor3d(1.0, 0.0, 1.0); // Couleur violette
				glVertex2f(Player->x, Player->y); 
				glVertex2f(Player->x, Player->y+1); 
				glVertex2f(Player->x+1, Player->y+0.5);
			}
			glEnd();
			//affichage mobile
			struct mobile *temp=mob->first;
			while(temp!=NULL)
			{
				if(temp->alive==true&&(temp->value==2||temp->value==6))
				{
				glPushMatrix();
				glTranslatef(temp->translation,0.0,0.0);
				glBegin(GL_QUADS);
					glColor3d(1.0, 0.5, 0.0); // Couleur orange
					glVertex2f(temp->x, temp->y);
					glVertex2f(temp->x, temp->y+1);
					glVertex2f(temp->x+1, temp->y+1);
					glVertex2f(temp->x+1, temp->y);
				glEnd();
				glPopMatrix();
				}
				else if(temp->alive==true&&(temp->value==1||temp->value==5))
				{
				glPushMatrix();
				glTranslatef(temp->translation,0.0,0.0);
				glBegin(GL_QUADS);
					glColor3d(1.0, 0.0, 0.0); // Couleur rouge
					glVertex2f(temp->x, temp->y);
					glVertex2f(temp->x, temp->y+1);
					glVertex2f(temp->x+1, temp->y+1);
					glVertex2f(temp->x+1, temp->y);
				glEnd();
				glPopMatrix();
				}
				else if(temp->alive==true&&(temp->value==3||temp->value==7))
				{
				glPushMatrix();
				glTranslatef(temp->translation,0.0,0.0);
				glBegin(GL_QUADS);
					glColor3d(0.0, 0.0, 1.0); // Couleur bleu
					glVertex2f(temp->x, temp->y);
					glVertex2f(temp->x, temp->y+1);
					glVertex2f(temp->x+1, temp->y+1);
					glVertex2f(temp->x+1, temp->y);
				glEnd();
				glPopMatrix();
				}
				else if(temp->alive==true&&(temp->value==4||temp->value==8))
				{
				glPushMatrix();
				glTranslatef(temp->translation,0.0,0.0);
				glBegin(GL_QUADS);
					glColor3d(0.0, 1.0, 0.0); // Couleur vert
					glVertex2f(temp->x, temp->y);
					glVertex2f(temp->x, temp->y+1);
					glVertex2f(temp->x+1, temp->y+1);
					glVertex2f(temp->x+1, temp->y);
				glEnd();
				glPopMatrix();
				}
				temp=temp->next;
			}
			//affichage thorn
			for(i=0;i<5;i++)
			{
				if(Thorn[i].alive==true)
				{
					if(Thorn[i].angle==0)
					{
						glPushMatrix();
						glTranslatef(Thorn[i].speed,0.0,0.0);
						glBegin(GL_LINES);
							glColor3d(0.0, 0.0, 0.0); // Couleur noir
							glVertex2f(Thorn[i].x+0.5, Thorn[i].y+0.5);
							glVertex2f(Thorn[i].x+0.2, Thorn[i].y+0.5);
						glEnd();
						glPopMatrix();
					}
					else if(Thorn[i].angle==90)
					{
						glPushMatrix();
						glTranslatef(0.0,Thorn[i].speed,0.0);
						glBegin(GL_LINES);
							glColor3d(0.0, 0.0, 0.0); // Couleur noir
							glVertex2f(Thorn[i].x+0.5, Thorn[i].y+0.5);
							glVertex2f(Thorn[i].x+0.5, Thorn[i].y+0.2);
						glEnd();
						glPopMatrix();
					}
					else if(Thorn[i].angle==180)
					{
						glPushMatrix();
						glTranslatef(Thorn[i].speed,0.0,0.0);
						glBegin(GL_LINES);
							glColor3d(0.0, 0.0, 0.0); // Couleur noir
							glVertex2f(Thorn[i].x+0.5, Thorn[i].y+0.5);
							glVertex2f(Thorn[i].x+0.8, Thorn[i].y+0.5);
						glEnd();
						glPopMatrix();
					}
					else if(Thorn[i].angle==270)
					{
						glPushMatrix();
						glTranslatef(0.0,Thorn[i].speed,0.0);
						glBegin(GL_LINES);
							glColor3d(0.0, 0.0, 0.0); // Couleur noir
							glVertex2f(Thorn[i].x+0.5, Thorn[i].y+0.5);
							glVertex2f(Thorn[i].x+0.5, Thorn[i].y+0.8);
						glEnd();
						glPopMatrix();
					}
				}
			}
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

//faire correspondre à la quantité de thorn en jeu ET activer la fonction de dessin tant que thorn->first!=NULL