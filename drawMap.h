#ifndef _DRAWMAP_H_
#define _DRAWMAP_H_

#include "loadMap.h"

void tostring(char[],int);
void enterName();
void timer(int);
void drawObject(float x, float y, int tile, float sizeX, float sizeY, float translation);
void drawColoredSquare(float x, float y, float sizeX, float sizeY, float translation, float r, float g, float b);
void drawBitmapText(char *string,float x,float y);
void drawMap(char map[][NbCol]);



#endif
