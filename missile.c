#include "missile.h"
#include "loadMap.h"
#include <stdlib.h>

void spawnMissile() 
{
    time_t currentTime=time(NULL);
    double timeSinceLastSpawn=difftime(currentTime,Missile->lastTimeSpawn);

    
    if (timeSinceLastSpawn>=20.0&&Missile->alive==false) 
    {
        Missile->alive=true;
        int edgeBoard=rand()%4; // choisir un bord aléatoire (0: haut, 1: bas, 2: gauche, 3: droite)

        switch (edgeBoard)
        {
            case 0: // haut
                Missile->y=27;
                Missile->x=rand()%29-1; // position aléatoire entre -1 et 27 inclus
                break;
            case 1: // bas
                Missile->y=-1;
                Missile->x=rand()%29-1;
                break;
            case 2: // gauche
                Missile->x=-1;
                Missile->y=rand()%29-1;
                break;
            case 3: // droite
                Missile->x=27;
                Missile->y=rand()%29-1;
                break;
        }

        Missile->lastTimeSpawn=currentTime;
    }
}
