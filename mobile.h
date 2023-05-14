#ifndef _MOBILE_H_
#define _MOBILE_H_

#include <time.h>

struct mobileChain
{
    unsigned int size; //taille du tableau
    struct mobile *first; //pointeur vers le premier objet
};

struct mobile
{
    int value; //valeur de l'objet
    float x; //position en X
    float y; //position en Y
    float translation; //translation de l'objet
    float speed; //vitesse de l'objet
    int angle; //angle de l'objet
    bool alive; //si l'objet est en vie ou non
    bool shoot; //si l'objet tir ou non
    bool freeze; //si l'objet est gelé ou non
    time_t freezeTimestamp; //timestamp de la dernière fois que l'objet a été gelé
    int shootAngle; //angle du tir
    struct mobile *next; //pointeur vers le prochain objet
    struct mobile *prev; //pointeur vers le précedent objet
};


struct mobileChain *mkmobileChaine();
void readValue(struct mobileChain *mob);
int mobileElem(struct mobileChain *mob);
int mobileElem2(struct mobileChain *wood);
void pushmChain(struct mobileChain *mob,int j,int k,int l,int m,int n);
void popmChain(struct mobileChain *mob);
#endif