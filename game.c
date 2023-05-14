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
#include "leaderboard.h"

// Toutes les variables externes sont définies dans loadMap.h
// Si dessous les variables globales du fichier game.c
bool UP = false;
bool LEFT = false;
bool RIGHT = false;
bool DOWN = false;
bool FIRE = false;
bool ELECFIRE = false;
bool PAUSE = false;
bool leaderBoardupdate = false;
bool loadLeaderBoard = false;
bool oneTimeMidBonus = false;
int mobileGeneration=0;
int deadMob=0;
int deadMobY=0;
int deadMobAngle=0;
unsigned int menu=0;
unsigned int difficulty=1;
leaderboardEntry top5[5];


// variables nécessaires pour la gestion de l'animation de mort.
clock_t death_start_time;
bool death_animation_started = false;

// variables nécessaires pour la gestion de l'introduction du nom du joueur et l'affichage de celui-ci en temps réel.
bool inputName = false;
int inputNameIndex = 0;

// variable nécessaires pour la gestion du temps qui s'écoule dès que le jeu commence.
time_t startGame;
int timeGame = 0;

void checkMobileFreeze() 
{
    struct mobile *tmp=mob->first;
    while(tmp!=NULL) 
	{
        if(tmp->freeze) 
		{
            time_t now;
            time(&now);
            if(difftime(now,tmp->freezeTimestamp)>2.0) 
			{
                tmp->freeze=false;
            }
        }
        tmp=tmp->next;
    }
}

void testFruitsEating()
{
	struct mobile *temp=bonusFruits->first;
	while(temp!=NULL)
	{
		if(Player->x==temp->x&&Player->y==temp->y)
		{
			Player->score+=100;
			temp->alive=false;
		}
		temp=temp->next;
	}
}

void generateFruits(struct mobileChain *bonusFruits)
{
	static time_t lastFruitTime=0;
	time_t currentTime=time(NULL);

	if (currentTime - lastFruitTime >= 20) 
	{
		int i;
		for(i=0;i<5;i++)
		{
			int randLin=11+rand()%((23+1)-11); // génère un nombre aléatoire entre 11 et 23 inclus
			int randCol=3+rand()%((23+1)-3); // génère un nombre aléatoire entre 3 et 23 inclus
			pushmChain(bonusFruits, 10, randLin, randCol, 0, 0);
		}
		lastFruitTime = currentTime;
	}
}

void scoreAndDeathCheck()
{
	if(wood!=NULL)
	{
		int onwood=0;
		struct mobile *temp2=wood->first;
		while(temp2!=NULL)
		{
			if(temp2->value==1||temp2->value==3||temp2->value==5||temp2->value==7)
			{
				if(Player->x>=temp2->x+temp2->translation&&Player->x<=temp2->x+5+temp2->translation&&Player->y==temp2->y) 
				{
					onwood++;
				}
			}
			else if(temp2->value==2||temp2->value==4||temp2->value==6||temp2->value==8)
			{
				if(Player->x>=temp2->x+temp2->translation&&Player->x<=temp2->x+5+temp2->translation&&Player->y==temp2->y) 
				{
					onwood++;
				}
			}
		temp2=temp2->next;
		}
		if((onwood==0&&Player->y>1&&Player->y<6)||(onwood==0&&Player->y>6&&Player->y<11))
		{
			Player->alive=false;
		}
		if(Player->y==6||Player->y==1)
		{
			if(oneTimeMidBonus==false&&Player->y==6)
			{
				Player->score+=1000;
				oneTimeMidBonus=true;
			}
			else if(Player->y==1)
			{
				Player->score+=1000;
				Player->y=24;
				Player->x=12;
				Missile->alive=false;
				oneTimeMidBonus=false;
			}
		}
	}
}

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
							if (tmp0->value==13)
							{
								tmp->freeze=true;
								time(&(tmp->freezeTimestamp)); // enregistrer le temps actuel
							}
							else
							{
								tmp->alive=false;
								deadMobY=tmp->y;
								deadMob=tmp->value;
								deadMobAngle=tmp->angle;
								Player->score+=100;
								printf("score: %d\n",Player->score);
							}
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
							if (tmp0->value==13)
							{
								tmp->freeze=true;
								time(&(tmp->freezeTimestamp)); // enregistrer le temps actuel
							}
							else
							{
								tmp->alive=false;
								deadMobY=tmp->y;
								deadMob=tmp->value;
								deadMobAngle=tmp->angle;
								Player->score+=100;
								printf("score: %d\n",Player->score);
							}
						}
						else if(tmp0->x+tmp0->translation<1||tmp0->x+tmp0->translation>24)
						{
							tmp0->alive=false;
						}
					}
				}
			}
			if(tmp0->alive&&tmp0->x-0.5+tmp0->translation>=Missile->x&&tmp0->x+tmp0->translation<=Missile->x+1&&tmp0->y>=Missile->y&&tmp0->y<=Missile->y+1)
			{
				Player->score+=1000;
				tmp0->alive=false;
				Missile->alive=false;
				Missile->x=-1;
				Missile->y=-1;
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
			if(Player->x<tmp->x+tmp->translation&&tmp->x+tmp->translation<Player->x+0.6&&Player->y==tmp->y)
			{
				Player->alive=false;
			}
		}
		if (mobShoot->size > 0)
		{
			struct mobile *tmp3=mobShoot->first;
			while(tmp3!=NULL)
			{
				if(Player->x<tmp3->x+tmp3->translation&&tmp3->x+tmp3->translation<Player->x+0.6&&Player->y==tmp3->y)
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
	if(Missile->alive&&Player->x-0.6<Missile->x&&Missile->x<Player->x+0.6&&Player->y-0.6<Missile->y&&Missile->y<Player->y+0.6)
	{
		Player->alive=false;
		Missile->alive=false;
	}
}

void mobAutoShoot() 
{
	//Tir des mobiles si le joueur est en vue
    struct mobile *tmp=mob->first;
    while (tmp!=NULL) 
	{
        if(tmp->alive==true&&tmp->freeze==false) 
		{
            if(tmp->y==Player->y&&tmp->shoot!=true)
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
        tmp=tmp->next;
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
			case'e':
				ELECFIRE = true;
				break;
			case'p':
				PAUSE = !PAUSE;
				break;
		}
	}
}

void game(char map[][NbCol]) //Fonction gérant toutes les mécaniques du jeu
{
	if(loadLeaderBoard==false)
	{
		int count=readLeaderboard("leaderboard.txt",top5,5); //On lit le leaderboard et on le stock pour affichage
		loadLeaderBoard=true;
		for(int i=0;i<count;i++) 
		{
        	printf("%d. %s: %d\n", i+1, top5[i].name, top5[i].score);
    	}
	}
	if(menu==1)
	{
		if(PAUSE==false)
		{
			if(mobileGeneration==0)
			{
				srand(time(NULL));
				Missile->lastTimeSpawn=time(NULL);
				mob=mkmobileChaine();
				wood=mkmobileChaine();
				playerShoot=mkmobileChaine();
				mobShoot=mkmobileChaine();
				bonusFruits=mkmobileChaine();
				mobileElem2(wood);
				mobileElem(mob);	//créer les mobiles
				mobileGeneration=1;
			}
			//readValue(thorn);		//afficher les valeurs des projectiles
			drawMap(map);			//afficher la carte
			if(Player->alive==true)
			{
				if(FIRE==true)
				{
					pushmChain(playerShoot,8,Player->y,Player->x,Player->angle,0);
					FIRE=false;
				}
				if(ELECFIRE==true)
				{
					pushmChain(playerShoot,12,Player->y,Player->x,Player->angle,0);
					ELECFIRE=false;
				}
				if (LEFT==true)
				{
					
					moveLeft(map);		//va se déplacer vers la gauche si on appuie sur q
					LEFT=false;
					
				}
				if (RIGHT==true)
				{
					
					moveRight(map);		//va se déplacer vers la droite si on apppuie sur d
					RIGHT=false;
				}
				if (UP==true)
				{
					moveUp(map);			//va se déplacer vers la droite si on apppuie sur z
					UP=false;
				}
				
				if (DOWN==true)
				{
					
					moveDown(map);		//va se déplacer vers la droite si on apppuie sur s
					DOWN=false;
				}
				mobAutoShoot();			//les mobiles tirent sur le joueur
				testCollision();		//test si le joueur est en collision avec un mobile
				testFruitsEating();		//test si le joueur mange un fruit
			}
			generateFruits(bonusFruits);		//génère des fruits
			spawnMissile();		//génère des missiles
			testCollisionPShoot(); //test si un véhicule est en collision avec un projectile du joueur
			popmChain(mob);			//supprime les mobiles qui sont morts
			popmChain(mobShoot);	//supprime les projectiles qui sont morts
			popmChain(playerShoot);//supprime les projectiles du joueur qui sont morts
			popmChain(bonusFruits);	//supprime les fruits qui sont mangés
			death_animation();		//animation de mort
			checkMobileFreeze();	//vérifie si les mobiles sont gelés
			scoreAndDeathCheck();	//vérifie si le joueur est mort et met à jour le score
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
	else
	{
		drawMap(map);
	}
	if(Player->vie==0)
	{
		if(leaderBoardupdate==false)
		{
			const char *leaderboard = "leaderboard.txt";
			writePlayerScore(leaderboard); //écrit le score du joueur dans le fichier leaderboard
			leaderBoardupdate=true;
			sortLeaderboard(leaderboard); //trie le leaderboard
		}
	}
}