#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "loadMap.h"
#include "mobile.h"

bool loadMap(char map[][NbCol])		//fonction qui ouvre le fichier txt et charge la carte dans le tableau et attribue les coordonnées du joueur et des mobiles.
{
    FILE *f = NULL;
    f = fopen("base_map.txt", "r");
    if(f == NULL)
    {
        printf("file empty");
        return false;
    }
    char c;
	int i,j;


	
	for(j = 0; j < NbLin; j++)
		{
			for(i = 0; i <= NbCol; i++)
			{		
				c = fgetc(f);
				map[i][j] = c;
			}
			fgetc(f);
		}

	    fclose(f);
		
	    printf("\n");

		for(j = 0; j < NbLin; j++)
		{
			for(i = 0; i < NbCol; i++)
			{
				printf("%c", map[i][j]);
				if((map[i][j])=='6')
				{
					Player->x=i;
					Player->y=j;
					Player->angle=0;
					Player->alive=true;
					Player->vie=3;
				}
			}
			printf("\n");
		}
 return true;
}