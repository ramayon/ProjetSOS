#ifndef _DRAWMAP_H_
#define _DRAWMAP_H_

#include "loadMap.h"

void tostring(char[],int);
void timer(int);
void drawObject(int x, int y, int tile, int size, float translation);
void drawBitmapText(char *string,float x,float y);
void drawMap(char map[][NbCol]);



#endif
