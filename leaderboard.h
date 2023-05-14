#ifndef _LEADERBOARD_H_
#define _LEADERBOARD_H_

typedef struct 
{
    char name[20];
    int score;
}leaderboardEntry;

void sortLeaderboard(const char *leaderboard);
void writeLeaderboard(const char *leaderboard, const leaderboardEntry *entries, int count);
int compareLeaderboardEntries(const void *a, const void *b);
int readLeaderboard(const char *leaderboard,leaderboardEntry *entries,int maxEntries);
void writePlayerScore(const char *leaderboard);

#endif