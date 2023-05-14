#ifdef __APPLE_CC__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "leaderboard.h"
#include "loadMap.h"

void sortLeaderboard(const char *leaderboard) 
{
    leaderboardEntry entries[20];
    int count=readLeaderboard(leaderboard,entries,sizeof(entries)/sizeof(entries[0]));
    qsort(entries,count,sizeof(entries[0]),compareLeaderboardEntries);
    writeLeaderboard(leaderboard, entries, count);
}

void writeLeaderboard(const char *leaderboard, const leaderboardEntry *entries,int count) 
{
    FILE *file = fopen(leaderboard, "w");
    if (!file) 
	{
        perror("Error opening file");
        return;
    }

    for (int i=0;i<count;i++) 
	{
        fprintf(file,"%s %d\n",entries[i].name,entries[i].score);
    }

    fclose(file);
}

int compareLeaderboardEntries(const void *a, const void *b) 
{
    const leaderboardEntry *entryA=(const leaderboardEntry *)a;
    const leaderboardEntry *entryB=(const leaderboardEntry *)b;
    return entryB->score-entryA->score;
}

int readLeaderboard(const char *leaderboard,leaderboardEntry *entries,int maxEntries) 
{
    FILE *file = fopen(leaderboard, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    int count = 0;
    while (count<maxEntries&&fscanf(file,"%19s %d",entries[count].name,&entries[count].score)==2) 
	{
        count++;
    }

    fclose(file);
    return count;
}

void writePlayerScore(const char *leaderboard) 
{
    FILE *file = fopen(leaderboard, "a");
    if (file == NULL) 
	{
        perror("Error opening file");
        return;
    }
    fprintf(file,"%s %d\n",Player->name,Player->score);
    fclose(file);
}