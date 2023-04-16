#include <GL/gl.h>
#include <GL/glu.h>
#include <FreeImage.h> // Include the FreeImage library

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "texture.h"

// Texture IDs array

GLuint textureIds[NUM_TEXTURES];

// Initialize and load the textures
void initTextures()
{
    const char *textureFiles[NUM_TEXTURES] = {"concrete.bmp", "water.bmp","grass.bmp","wood.bmp","Hedgehog.left.png","Hedgehog.up.png","Hedgehog.down.png","Hedgehog.right.png","car1.png","car2.png","car3.png","car4.png","Background_start_menu.png","SettingBackground.png","BackgroundKeyFunc.png","metal.png","Hedgehog.dead.png"};
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(textureFiles[i], 0);
    FIBITMAP *image = FreeImage_Load(format, textureFiles[i],0);
    if (image == NULL)
    {
        fprintf(stderr, "Failed to load texture image: %s\n", textureFiles[i]);
        return;
    }

    FIBITMAP *image32Bit = FreeImage_ConvertTo32Bits(image);

    glGenTextures(1, &textureIds[i]);
    glBindTexture(GL_TEXTURE_2D, textureIds[i]);
    int width = FreeImage_GetWidth(image32Bit);
    int height = FreeImage_GetHeight(image32Bit);
    GLubyte *texels = (GLubyte *)FreeImage_GetBits(image32Bit);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, texels);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free the image data
    FreeImage_Unload(image32Bit);
    FreeImage_Unload(image);
    }
}