#ifndef _MOBILE_H_
#define _MOBILE_H_

struct mobileChain
{
    unsigned int size; //taille du tableau
    struct mobile *first; //pointeur vers le premier objet
};

struct mobile
{
    int value; //valeur de l'objet
    float x; //position en X
    int y; //position en Y
    float translation; //translation de l'objet
    bool alive; //si l'objet est en vie ou non
    struct mobile *next; //pointeur vers le prochain objet
    struct mobile *prev; //pointeur vers le précedent objet
};


struct mobileChain *mkmobileChaine();
void readValue(struct mobileChain *mob);
int mobileElem(struct mobileChain *mob);
int mobileElem2(struct mobileChain *wood);
void pushmChain(struct mobileChain *mob,int j,int k,int l);
void popmChain(struct mobileChain *mob);
#endif