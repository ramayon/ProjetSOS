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


// Toutes les variables externes sont définies dans loadMap.h
// Si dessous les variables globales du fichier game.c
bool UP = false;
bool LEFT = false;
bool RIGHT = false;
bool DOWN = false;
bool FIRE = false;
bool PAUSE = false;
int mobileGeneration=0;
int deadMob=0;
int deadMobY=0;
int deadMobAngle=0;
unsigned int menu=0;
unsigned int difficulty=1;

// variables nécessaires pour la gestion de l'animation de mort.
clock_t death_start_time;
bool death_animation_started = false;

// variables nécessaires pour la gestion de l'introduction du nom du joueur et l'affichage de celui-ci en temps réel.
bool inputName = false;
int inputNameIndex = 0;

// variable nécessaires pour la gestion du temps qui s'écoule dès que le jeu commence.
time_t startGame;
int timeGame = 0;

void death_animation()
{
    if (Player->alive==false&&death_animation_started==false) 
	{
        death_start_time=clock();
        death_animation_started=true;
    }
    if (death_animation_started==true) 
	{
        double elapsedTime=(double)(clock()-death_start_time)/CLOCKS_PER_SEC;
		//contrôle la durée de mort du joueur
        if (elapsedTime>=2.0) 
		{
            // Replace le joueur au point de départ après 2 secondes.
			Player->vie-=1;
			Player->y=24;
			Player->x=12;
            Player->alive=true;
            death_animation_started=false;
        }
    }
}

void testCollisionPShoot()
{
	struct mobile *tmp=mob->first;
	while(tmp!=NULL)
	{
		struct mobile *tmp0=playerShoot->first;
		while(tmp0!=NULL)
		{
			if(tmp->alive==true)
			{
				if(tmp0->alive==true)
				{
					if(tmp0->angle==90||tmp0->angle==270) //haut bas
					{
						if((tmp0->x+0.5)>(tmp->x+tmp->translation)&&(tmp0->x+0.5)<(tmp->x+tmp->translation+1)&&(tmp0->y+0.3+tmp0->translation)<tmp->y+1&&(tmp0->y+0.3+tmp0->translation)>tmp->y)
						{
							tmp0->alive=false;
							tmp->alive=false;
							deadMobY=tmp->y;
							deadMob=tmp->value;
							deadMobAngle=tmp->angle;
							Player->score+=100;
							printf("score: %d\n",Player->score);
						}
						else if(tmp0->y+tmp0->translation<1||tmp0->y+tmp0->translation>24)
						{
							tmp0->alive=false;
						}
					}
					else if(tmp0->angle==0||tmp0->angle==180) //gauche droite
					{
						if((tmp0->x+0.3+tmp0->translation)>(tmp->x+tmp->translation)&&(tmp0->x+0.3+tmp0->translation)<(tmp->x+tmp->translation+1)&&(tmp0->y+0.5)<tmp->y+1&&(tmp0->y+0.5)>tmp->y)
						{
							tmp0->alive=false;
							tmp->alive=false;
							deadMobY=tmp->y;
							deadMob=tmp->value;
							deadMobAngle=tmp->angle;
							Player->score+=100;
							printf("score: %d\n",Player->score);
						}
						else if(tmp0->x+tmp0->translation<1||tmp0->x+tmp0->translation>24)
						{
							tmp0->alive=false;
						}
					}
				}
			}
			tmp0=tmp0->next;
		}
		tmp=tmp->next;
	}
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
				Player->alive=false;
			}
		}
		else if(tmp->value==3||tmp->value==4||tmp->value==7||tmp->value==8)
		{
			if(Player->x<tmp->x+tmp->translation&&tmp->x+tmp->translation<Player->x+0.6&& Player->y==tmp->y)
			{
				Player->alive=false;
			}
		}
		if (mobShoot->size > 0)
		{
			struct mobile *tmp3=mobShoot->first;
			while(tmp3!=NULL)
			{
				if(Player->x<tmp3->x+tmp3->translation&&tmp3->x+tmp3->translation<Player->x+0.6&& Player->y==tmp3->y)
				{
					Player->alive=false;
					tmp3->alive=false;
					struct mobile *mobShooter = mob->first;
            		while (mobShooter!=NULL) 
					{
                		if (mobShooter->y==tmp3->y) 
						{
                    		mobShooter->shoot=false;
                		}
                		mobShooter=mobShooter->next;
            		}
				}
				tmp3=tmp3->next;
			}
		}
	tmp=tmp->next;
	}
}

void mobAutoShoot() 
{
	//Tir des mobiles si le joueur est en vue
    struct mobile *tmp = mob->first;
    while (tmp != NULL) 
	{
        if (tmp->alive == true) 
		{
            if (tmp->y==Player->y&&tmp->shoot!=true)
			{
				int ShootAngle=0;
				if((tmp->x+tmp->translation)<Player->x)
				{
					ShootAngle=1;
				}
				else if((tmp->x+tmp->translation)>Player->x)
				{
					ShootAngle=-1;
				}
                pushmChain(mobShoot,9,tmp->y,tmp->x+tmp->translation,tmp->angle,ShootAngle);
				tmp->shoot=true;
            }
        }
        tmp = tmp->next;
    }
	//Destruction du tir s'il atteint les limites de la carte
	struct mobile *tmp3=mobShoot->first;
    while (tmp3!=NULL) 
	{
        if (tmp3->alive==true) 
		{
            if (tmp3->x+tmp3->translation<1||tmp3->x+tmp3->translation>25) 
			{
                tmp3->alive=false;
				struct mobile *mobShooter = mob->first;
				while (mobShooter!=NULL) 
				{
					if (mobShooter->y==tmp3->y) 
					{
						mobShooter->shoot=false;
					}
					mobShooter=mobShooter->next;
				}
            } 
        }
        tmp3=tmp3->next;
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
                               if(x>230&&x<465&&y>450&&y<480)
							   {
								exit(0); //quitter le jeu
							   }
							   else if(x>230&&x<465&&y>265&&y<295)
							   {
								   menu=4; //afficher le menu de saisie du nom
								   inputName=true; //entrer son nom
							   }
							   else if(x>230&&x<465&&y>325&&y<355)
							   {
								   menu=2; //afficher le menu paramètre
							   }
							   else if(x>230&&x<465&&y>385&&y<415)
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
							if(menu==2)
							{
								if(x>760&&x<900&&y>345&&y<360)
								{
									difficulty=1; //choix de la difficulté facile
								}
								else if(x>760&&x<915&&y>405&&y<425)
								{
									difficulty=2; //choix de la difficulté normale
								}
								else if(x>760&&x<990&&y>470&&y<485)
								{
									difficulty=3; //choix de la difficulté difficile
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
	if(inputName==true)
	{
		if(key=='\r'||key=='\n') //Gère la validation du nom en prenant en compte les systèmes windows qui utilisent "\r\n" et les systèmes linux qui utilisent "\n"
		{
			inputName=false;
			Player->name[inputNameIndex]='\0'; //On met un caractère de fin de chaine à la fin du nom
			menu=1;
			startGame=time(NULL);
		}
		else if(key=='\b') //Gère la suppression d'un caractère
		{
			if(inputNameIndex>0)
			{
				inputNameIndex--;
				Player->name[inputNameIndex]=' ';
			}
		}
		else if(inputNameIndex<sizeof(Player->name)) //Gère l'ajout d'un caractère
		{
			Player->name[inputNameIndex]=key;
			inputNameIndex++;
		}
	}
	else
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
				PAUSE = !PAUSE;
				break;
		}
	}
}

void game(char map[][NbCol]) //Fonction gérant toutes les mécaniques du jeu
{
	if(PAUSE==false)
	{
		if(mobileGeneration==0)
		{
			mob=mkmobileChaine();
			wood=mkmobileChaine();
			playerShoot=mkmobileChaine();
			mobShoot=mkmobileChaine();
			mobileElem2(wood);
			mobileElem(mob);	//créer les mobiles
			mobileGeneration=1;
		}

		if(FIRE==true)
		{
			pushmChain(playerShoot,8,Player->y,Player->x,Player->angle,0);
			FIRE=false;
		}
		//readValue(thorn);		//afficher les valeurs des projectiles
		drawMap(map);			//afficher la carte
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
		if(Player->alive==true)
		{
			mobAutoShoot();			//les mobiles tirent sur le joueur
			testCollision();		//test si le joueur est en collision avec un mobile
		}
		testCollisionPShoot(); //test si un véhicule est en collision avec un projectile du joueur
		popmChain(mob);			//supprime les mobiles qui sont morts
		popmChain(mobShoot);	//supprime les projectiles qui sont morts
		popmChain(playerShoot);//supprime les projectiles du joueur qui sont morts
		death_animation();		//animation de mort
		if(deadMob!=0)
		{
			if(deadMob==1||deadMob==2||deadMob==5||deadMob==6)
			{
				pushmChain(mob,deadMob-1,deadMobY,24,0,0);	//réanime les mobiles qui sont morts
			}
			else if(deadMob==3||deadMob==4||deadMob==7||deadMob==8)
			{
				pushmChain(mob,deadMob-1,deadMobY,1,0,0);	//réanime les mobiles qui sont morts
			}
			
			deadMob=0;
		}
		timeGame=(int)difftime(time(NULL),startGame); //temps de jeu écoule calculé en fonction de la différence entre le temps actuel et le temps de début de partie
		glutPostRedisplay();
	}
	else
	{
		drawMap(map);
	}
}