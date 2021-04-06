#include<stdbool.h>
#include"gamecore.h"
#include"stats.h"
#ifndef INTERHACE_H
#define INTERFACE_H

player_l* startload(player_l** currentplayer);

player_l* changeplayer(player_l* playerlist);

void menu(void);

player_l* newplayer(player_l* playerlist);

void game(player_l* currentplayer);

void add(int(*board)[9], int *history);

void emptycell(int(*board)[9], int *history);

void drawboard(int(*board)[9]);

bool checkboard(int(*board)[9]);

void endgame(player_l* currentplayer, int time, int diff);

void recallmoves(int* history);

void close(player_l*playerlist);

void load(int(*board)[9], int*history, int*time, int *diff);

void save(int(*board)[9], int* history, int time, int diff);

void printhelp(void);

void printplayerlist(player_l* playerlist);

void printgamelist(game_l* gamelist);
#endif

