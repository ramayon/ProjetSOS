#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <FreeImage.h> // Include the FreeImage library

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_TEXTURES 17
extern GLuint textureIds[NUM_TEXTURES];
void initTextures();

#endif