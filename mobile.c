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

#include "loadMap.h"
#include "drawMap.h"
#include "hedgehog.h"
#include "mobile.h"

void readValue(struct mobileChain *mob)
{
    struct mobile *temp=mob->first;
    while(temp!=NULL)
    {
        printf("%f",temp->x);
        temp=temp->next;
    }
}

void pushmChain(struct mobileChain *mob,int j, int k, int l,int m,int n)
{
	struct mobile *newMob=malloc(sizeof(struct mobile));
	newMob->value=j+1;
	newMob->y=k;
	newMob->x=l;
    newMob->translation=0;
    newMob->speed=0.05+(float)rand()/(float)(RAND_MAX/(0.2-0.05)); //vitesse aléatoire entre 0.05 et 0.2
    newMob->angle=m;
    newMob->alive=true;
    newMob->shoot=false;
    newMob->freeze=false;
    newMob->shootAngle=n;
	newMob->next=mob->first;
    newMob->prev=NULL;
    if(mob->first!=NULL)
    {
        mob->first->prev=newMob;
    }
	mob->first=newMob;
	mob->size++;
}

void popmChain(struct mobileChain *mob)
{
    struct mobile *temp=mob->first;
    while(temp!=NULL)
    {
        if(temp->alive==false)
        {
            if(temp->prev!=NULL)
            {
                temp->prev->next=temp->next;
            }
            else
            {
                mob->first=temp->next;
            }
            if(temp->next!=NULL)
            {
                temp->next->prev=temp->prev;
            }
            free(temp);
            mob->size--;
        }
        temp=temp->next;
    }
}


struct mobileChain *mkmobileChaine()
{
    struct mobileChain *chain = malloc(sizeof(struct mobileChain));
    chain->size = 0;
    chain->first = NULL;
    return chain;
}

int mobileElem(struct mobileChain *mob)
{
    int i,j=0;
    int k=22;
    while(j<8)
    {
        for(i=0;i<2;i++,k--,j++)
        {		
            pushmChain(mob,j,k,25,0,0);
        }
        k--;
        for(i=0;i<2;i++,k--,j++)
        {
            pushmChain(mob,j,k,0,0,0);
        }
        k--;
    }
    return 1;
}

int mobileElem2(struct mobileChain *wood)
{
    int j=0;
    int k=10;
    int l=24;
    int m=1;
    while(j<4)
    {
        pushmChain(wood,j,k,l,0,0);
        j++;
        k--;
        pushmChain(wood,j,k,m,0,0);
        j++;
        k--;
    }
    k--;
    while(j<8)
    {
        pushmChain(wood,j,k,l,0,0);
        j++;
        k--;
        pushmChain(wood,j,k,m,0,0);
        j++;
        k--;
    }
    return 1;
}