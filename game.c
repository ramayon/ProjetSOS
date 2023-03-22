#ifdef __APPLE_CC__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "loadMap.h"
#include "drawMap.h"
#include "hedgehog.h"
#include "mobile.h"

bool UP = false;
bool LEFT = false;
bool RIGHT = false;
bool DOWN = false;
bool FIRE = false;
bool Pause = false;
unsigned int countThorn=0;
int mobileGeneration=0;
int deadMob=0;
int deadMobY=0;
unsigned int menu=0;

void choiceMenu()
{
	
}

void testCollision()
{
	struct mobile *tmp=mob->first;
	while(tmp!=NULL)
	{
		if(tmp->value==1||tmp->value==2||tmp->value==5||tmp->value==6)
		{
			if(Player->x<tmp->x+tmp->translation&&tmp->x+tmp->translation<Player->x+0.6&& Player->y==tmp->y)
			{
				Player->vie-=1;
				Player->y=25;
				Player->x=12;
			}
		}
		else if(tmp->value==3||tmp->value==4||tmp->value==7||tmp->value==8)
		{
			if(Player->x<tmp->x+tmp->translation&&tmp->x+tmp->translation<Player->x+0.6&& Player->y==tmp->y)
			{
				Player->vie-=1;
				Player->y=25;
				Player->x=12;
			}
		}
		tmp=tmp->next;
	}
	if(Player->vie==0)
	{
		Player->alive=false;
	}
}

void testCollisionThorn()
{
	struct mobile *tmp=mob->first;
	while(tmp!=NULL)
	{
		if(tmp->alive==true)
		{
			for(int i=0;i<5;i++)
			{
				if(Thorn[i].alive==true)
				{
					if(Thorn[i].angle==90||Thorn[i].angle==270) //haut bas
					{
						if((Thorn[i].x+0.5)>(tmp->x+tmp->translation)&&(Thorn[i].x+0.5)<(tmp->x+tmp->translation+1)&&(Thorn[i].y+0.3+Thorn[i].speed)<tmp->y+1&&(Thorn[i].y+0.3+Thorn[i].speed)>tmp->y)
						{
							Thorn[i].alive=false;
							tmp->alive=false;
							deadMobY=tmp->y;
							deadMob=tmp->value;
							countThorn--;
							Player->score+=100;
							printf("score: %d\n",Player->score);
						}
						else if(Thorn[i].y+Thorn[i].speed<1||Thorn[i].y+Thorn[i].speed>24)
						{
							Thorn[i].alive=false;
							countThorn--;
						}
					}
					else if(Thorn[i].angle==0||Thorn[i].angle==180) //gauche droite
					{
						if((Thorn[i].x+0.3+Thorn[i].speed)>(tmp->x+tmp->translation)&&(Thorn[i].x+0.3+Thorn[i].speed)<(tmp->x+tmp->translation+1)&&(Thorn[i].y+0.5)<tmp->y+1&&(Thorn[i].y+0.5)>tmp->y)
						{
							Thorn[i].alive=false;
							tmp->alive=false;
							deadMobY=tmp->y;
							deadMob=tmp->value;
							countThorn--;
							Player->score+=100;
							printf("score: %d\n",Player->score);
						}
						else if(Thorn[i].x+Thorn[i].speed<1||Thorn[i].x+Thorn[i].speed>24)
						{
							Thorn[i].alive=false;
							countThorn--;
						}
					}
				}
			}
		}
		tmp=tmp->next;
	}
}

void mouse(int bouton,int etat,int x,int y) 
{
  if(etat==GLUT_DOWN)
    switch(bouton) 
	{
      case GLUT_LEFT_BUTTON  :
	  						if(menu==0)
							{
                               if(x>385&&x<615&&y>463&&y<493)
							   {
								exit(0); //quitter le jeu
							   }
							   else if(x>385&&x<615&&y>279&&y<309)
							   {
								   menu=1; //lancer le jeu
							   }
							   else if(x>385&&x<615&&y>339&&y<369)
							   {
								   menu=2; //afficher le menu paramètre
							   }
							   else if(x>385&&x<615&&y>401&&y<431)
							   {
								   menu=3; //afficher les règles
							   }
							}
							if(menu==3||menu==2)
							{
								if(x>78&&x<386&&y>710&&y<739)
								{
									menu=0; //retour au menu principal
								}
							}
							   printf("%4d %4d\n",x,y); 
                               glutPostRedisplay();
                               break ;
      case GLUT_RIGHT_BUTTON : exit(0); //mettre le jeu en pause
                               break; 
	}
}

void Keyboard(unsigned char key, int x, int y)  // fonction allant gérer les input
{
	switch(key)
	{
		case 27:
			exit(0);

		case'z':
			UP = true;
			break;

		case'q':
			LEFT = true;
			break;

		case'd':
			RIGHT = true;
			break;

		case's':
			DOWN = true;
			break;
		case'a':
			FIRE = true;
			break;
		case'p':
			Pause=true;
			while(Pause==true)
			{
				switch(key)
				{
					case'p':
					Pause=false;
				}
			}
	}	
}

void game(char map[][NbCol]) //Fonction gérant toutes les mécaniques du jeu
{
	if(mobileGeneration==0)
	{
		mob=mkmobileChaine();
		wood=mkmobileChaine();
		mobileElem2(wood);
		mobileElem(mob);	//créer les mobiles
		mobileGeneration=1;
	}

	if(FIRE==true&&countThorn<5)
	{
		countThorn++;
		for(int i=0;i<countThorn;i++)
		{
			if(Thorn[i].alive==false)
			{
				Thorn[i].x=Player->x;
				Thorn[i].y=Player->y;
				Thorn[i].angle=Player->angle;
				Thorn[i].speed=0.4;
				Thorn[i].alive=true;
			}
		}
		FIRE=false;
	}
	choiceMenu(); //afficher le menu
	//readValue(thorn);		//afficher les valeurs des projectiles
	drawMap(map);			//afficher la carte
	glutKeyboardFunc(Keyboard);		// Fonction de glut gérant le clavier
    glutMouseFunc(mouse);			// Fonction de gestion de la souris
	if (LEFT == true)
	{
		
		moveLeft(map);		//va se déplacer vers la gauche si on appuie sur q
		LEFT = false;
		
	}
	if (RIGHT == true)
	{
		
		moveRight(map);		//va se déplacer vers la droite si on apppuie sur d
		RIGHT = false;
	}
	if (UP == true)
	{
		moveUp(map);			//va se déplacer vers la droite si on apppuie sur z
		UP = false;
	}
	
	if (DOWN == true)
	{
		
        moveDown(map);		//va se déplacer vers la droite si on apppuie sur s
		DOWN = false;
	}
	testCollision(mob);		//test si le joueur est en collision avec un mobile
	testCollisionThorn(mob); //test si un véhicule est en collision avec un projectile
	popmChain(mob);			//supprime les mobiles qui sont morts
	if(deadMob!=0)
	{
		if(deadMob==1||deadMob==2||deadMob==5||deadMob==6)
		{
			pushmChain(mob,deadMob-1,deadMobY,24);	//réanime les mobiles qui sont morts
		}
		else if(deadMob==3||deadMob==4||deadMob==7||deadMob==8)
		{
			pushmChain(mob,deadMob-1,deadMobY,1);	//réanime les mobiles qui sont morts
		}
		
		deadMob=0;
	}
	glutPostRedisplay();
}