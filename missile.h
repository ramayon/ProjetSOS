#ifndef _MISSILE_H_
#define _MISSILE_H_

#include <stdbool.h>
#include <time.h>

typedef struct missile
{
    bool alive; //si le missile est en vie ou non
    float x; //position en X
    float y; //position en Y
    time_t lastTimeSpawn; //timestamp de la dernière fois que le missile a été tiré
}missile;

void spawnMissile(); // Si plus de 20 secondes se sont écoulées et qu'aucun missile n'est actuellement en vie, on en fait apparaître un nouveau
#endif