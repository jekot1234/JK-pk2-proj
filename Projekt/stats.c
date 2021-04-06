#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include "gamecore.h"
#include "stats.h"

player_l* loadplayers(void) {

	FILE* load = fopen("players", "rb");

	int playercount;

	if (load == NULL) {
		return NULL;
	}

	fread(&playercount, sizeof(int), 1, load);

	if (playercount == 0) {
		fclose(load);
		return NULL;
	}

	player_l* playerlist = NULL;
	player_l* temp_p;
	player_l* prev_p = NULL;

	for (int i = 0; i < playercount; i++) {

		temp_p = malloc(sizeof(player_l));

		if (playerlist == NULL) {
			playerlist = temp_p;
		}

		if (prev_p != NULL) {
			prev_p->next = temp_p;
		}

		int size = 10;
		char* name = calloc(size * sizeof(char));
		char t = '\n';


		for (int i = 0; t != '\0'; i++) {

			fread(&t, sizeof(char), 1, load);

			if (i >= size) {
				size += 10;
				char* lname = realloc(name, size * sizeof(char));
				name = lname;
			}

			name[i] = t;
		}

		temp_p->name = name;
		temp_p->gamelist = NULL;

		fread(&temp_p->totalscore, sizeof(int), 1, load);
		fread(&temp_p->id, sizeof(int), 1, load);
		fread(&temp_p->gamecount, sizeof(int), 1, load);

		temp_p->next = NULL;

		prev_p = temp_p;
	}

	return playerlist;
}

game_l* loadgames(player_l* player) {

	FILE* load = fopen(player->name, "rb");

	if (load == NULL) {
		return NULL;
	}

	game_l* gamelist = NULL;
	game_l* temp_p;
	game_l* prev_p = NULL;

	for (int i = 0; i < player->gamecount; i++) {

		temp_p = malloc(sizeof(game_l));

		if (gamelist == NULL) {
			gamelist = temp_p;
		}

		if (prev_p != NULL) {
			prev_p->next = temp_p;
		}

		fread(&temp_p->time, sizeof(int), 1, load);
		fread(&temp_p->diff, sizeof(int), 1, load);
		fread(&temp_p->score, sizeof(int), 1, load);


		temp_p->next = NULL;

		prev_p = temp_p;
	}

	fclose(load);

	return gamelist;
}

void saveplayers(player_l* playerlist, int playercount) {

	FILE* save = fopen("players", "wb");

	if (save == NULL) {
		return;
	}

	fwrite(&playercount, sizeof(int), 1, save);

	if (playerlist == NULL) {
		fclose(save);
		return;
	}

	player_l* temp_p = playerlist;

	while (temp_p != NULL) {
		char t = '\n';

		for (int i = 0; t != '\0'; i++) {
			t = temp_p->name[i];
			fwrite(&t, sizeof(char), 1, save);
		}

		fwrite(&temp_p->totalscore, sizeof(int), 1, save);
		fwrite(&temp_p->id, sizeof(int), 1, save);
		fwrite(&temp_p->gamecount, sizeof(int), 1, save);

		temp_p = temp_p->next;
	}
	fclose(save);

	return;
}

void savegames(player_l* currentplayer) {

	FILE* save = fopen(currentplayer->name, "wb");

	if (save == NULL) {
		return;
	}

	game_l* temp_p = currentplayer->gamelist;

	while (temp_p != NULL) {

		fwrite(&temp_p->time, sizeof(int), 1, save);
		fwrite(&temp_p->diff, sizeof(int), 1, save);
		fwrite(&temp_p->score, sizeof(int), 1, save);

		temp_p = temp_p->next;
	}

	fclose(save);
	return;
}

player_l* addplayer(player_l* playerlist, char* name) {

	player_l* newplayer = malloc(sizeof(player_l));

	int id = 1;

	newplayer->name = name;
	newplayer->totalscore = 0;
	newplayer->gamecount = 0;
	newplayer->gamelist = NULL;
	newplayer->next = NULL;

	if (playerlist == NULL) {
		playerlist = newplayer;
		newplayer->id = id;
		return playerlist;
	}
	else {
		player_l* temp_p = playerlist;
		while (temp_p->next != NULL) {
			temp_p = temp_p->next;
			id++;
		}
		newplayer->id = ++id;
		temp_p->next = newplayer;
		return playerlist;
	}

}

game_l* addgame(game_l* gamelist, int time, int diff, int score) {

	game_l* newgame = malloc(sizeof(game_l));

	newgame->time = time;
	newgame->diff = diff;
	newgame->score = score;
	newgame->next = NULL;

	if (gamelist == NULL) {
		gamelist = newgame;
		return gamelist;
	}
	else {
		game_l* temp_p = gamelist;
		while (temp_p->next != NULL) {
			temp_p = temp_p->next;
		}
		temp_p->next = newgame;
		return gamelist;
	}

}

bool loadgame(int(*board)[9], int*history, int*time, char*filename, int *diff) {

	FILE* load = fopen(filename, "rb");

	if (load == NULL) {
		return false;
	}

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {

			fread(&board[i][j], sizeof(int), 1, load);

		}
	}
	int histcount;

	fread(&histcount, sizeof(int), 1, load);

	history[0] = histcount;

	for (int i = 1; i <= histcount; i++) {

		fread(&history[i], sizeof(int), 1, load);

	}

	fread(time, sizeof(int), 1, load);
	fread(diff, sizeof(int), 1, load);

	fclose(load);

	return true;
}

void savegame(int(*board)[9], int* history, int time, char*filename, int diff) {

	FILE* save = fopen(filename, "wb");

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {

			fwrite(&board[i][j], sizeof(int), 1, save);

		}
	}

	fwrite(history, sizeof(int), history[0] + 1, save);
	fwrite(&time, sizeof(int), 1, save);
	fwrite(&diff, sizeof(int), 1, save);


	fclose(save);
	return;
}

void freelists(player_l* playerlist) {

	player_l* tempp_p = playerlist;
	player_l* prevp_p;
	game_l* tempg_p;
	game_l* prevg_p;

	while (tempp_p != NULL) {
		prevp_p = tempp_p;
		tempp_p = tempp_p->next;

		tempg_p = prevp_p->gamelist;
		while (tempg_p != NULL) {
			prevg_p = tempg_p;
			tempg_p = tempg_p->next;

			free(prevg_p);
		}
		free(prevp_p->name);
		free(prevp_p);
	}
	return;
}