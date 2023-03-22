#include <GL/gl.h>
#include <GL/glu.h>
#include <FreeImage.h> // Include the FreeImage library

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "texture.h"

// Texture IDs array
#define NUM_TEXTURES 6
GLuint textureIds[NUM_TEXTURES];

// Initialize and load the textures
void initTextures()
{
    const char *textureFiles[NUM_TEXTURES] = {"concrete.bmp", "water.bmp","grass.bmp","wood.bmp"};

    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        FIBITMAP *image = FreeImage_Load(FIF_BMP, textureFiles[i], BMP_DEFAULT);
        if (image == NULL)
        {
            fprintf(stderr, "Failed to load texture image: %s\n", textureFiles[i]);
            return;
        }
        glGenTextures(1, &textureIds[i]);
        glBindTexture(GL_TEXTURE_2D, textureIds[i]);
        int width = FreeImage_GetWidth(image);
        int height = FreeImage_GetHeight(image);
        GLubyte *texels = (GLubyte *)FreeImage_GetBits(image);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, texels);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Free the image data
        FreeImage_Unload(image);
    }
}