#include "gamecore.h"
#ifndef STATS_H
#define STATS_H

typedef struct game {
	int time;
	int score;
	int diff;
	struct game *next;

}game_l;

typedef struct player {
	char* name;
	int totalscore;
	int id;
	int gamecount;
	struct game* gamelist;
	struct player* next;

}player_l;

player_l* loadplayers(void);

game_l* loadgames(player_l* player);

void savegames(player_l* currentplayer);

void saveplayers(player_l* playerlist, int playercount);

player_l* addplayer(player_l* playerlist, char* name);

game_l* addgame(game_l* gamelist, int time, int diff, int score);

bool loadgame(int(*board)[9], int*history, int*time, char*filename, int *diff);

void savegame(int(*board)[9], int* history, int time, char*filename, int diff);

void freelists(player_l* playerlist);

#endif
