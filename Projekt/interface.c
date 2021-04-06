#include <stdio.h>
#include <time.h> 
#include <stdbool.h> 
#include"gamecore.h"
#include"interface.h"
#include"stats.h"
#include"AI.h"

player_l* startload(player_l** currentplayer) {

	int choice;
	player_l* playerlist = NULL;
	player_l* temp;
	*currentplayer == NULL;

	playerlist = loadplayers();

	if (playerlist == NULL) {

		printf("Player list empty\n");

		playerlist = newplayer(playerlist);
		*currentplayer = playerlist;
		saveplayers(playerlist, 1);
	}
	else {

		temp = playerlist;

		while (temp != NULL) {
			temp->gamelist = loadgames(temp);

			temp = temp->next;
		}

		printf("Chose player by enternig corresponding number\n");

		printplayerlist(playerlist);

		while (*currentplayer == NULL) {

			choice = getchar() - '0';
			while ((getchar()) != '\n');

			player_l* temp_p = playerlist;

			while (temp_p != NULL) {
				if (temp_p->id == choice) {
					*currentplayer = temp_p;
					break;
				}
				else {
					temp_p = temp_p->next;
				}
			}

			if (*currentplayer == NULL) {
				printf("No player with such ID\n");
			}
		}
	}
	return playerlist;
}

void menu(void) {

	char choice;
	player_l* playerlist = NULL;
	player_l* currentplayer = NULL;

	playerlist = startload(&currentplayer);

	printf("Active player: %s\n", currentplayer->name);

	while (1) {

		printf("MAIN MENU: \n1. Start a game\n2. Show my games \n3. Players and scores \n4. Change player \n5. New Player \n0. Exit\n");

		choice = getchar();
		while ((getchar()) != '\n');

		switch (choice) {
		case '1':
			game(currentplayer);
			break;
		case '2':
			printgamelist(currentplayer->gamelist);
			break;
		case '3':
			printplayerlist(playerlist);
			break;
		case '4':
			currentplayer = changeplayer(playerlist);
			printf("Active player: %s\n", currentplayer->name);
			break;
		case '5':
			playerlist = newplayer(playerlist);
			currentplayer = changeplayer(playerlist);
			printf("Active player: %s\n", currentplayer->name);
			break;
		case '0':
			close(playerlist);
			return;
		}
	}
}

player_l* newplayer(player_l* playerlist) {

	char* name = calloc(sizeof(char), sizeof(char) * 10);
	int size = 10;
	char t;

	printf("Enter player's name:\n");

	for (int i = 0; ; i++) {

		t = getc(stdin);

		if (i >= size) {
			size += 10;
			char* lname = realloc(name, size * sizeof(char));
			name = lname;
		}

		if (t == '\n') {
			name[i] = '\0';
			break;
		}
		name[i] = t;
	}

	playerlist = addplayer(playerlist, name);

	return playerlist;
}

player_l* changeplayer(player_l* playerlist) {

	int choice;
	player_l* currentplayer = NULL;

	if (playerlist == NULL) {

		printf("Player list empty\n");

		playerlist = newplayer(playerlist);
		currentplayer = playerlist;
		saveplayers(playerlist, 1);
	}
	else {

		printf("Chose player by enternig corresponding number\n");

		printplayerlist(playerlist);

		while (currentplayer == NULL) {

			choice = getchar() - '0';
			while ((getchar()) != '\n');

			player_l* temp_p = playerlist;

			while (temp_p != NULL) {
				if (temp_p->id == choice) {
					currentplayer = temp_p;
					break;
				}
				else {
					temp_p = temp_p->next;
				}
			}

			if (currentplayer == NULL) {
				printf("No player with such ID\n");
			}
		}
	}
	return currentplayer;
}


void createboard(int(*board)[9], int* diff) {

	char choice;
	char filename[21] = { '/0' };
	char t;
	bool loaded = false;

	printf("NEW GAME\n1. Load sudoku from text file\n2. Generate random sudoku\n");

	choice = getchar();
	while ((getchar()) != '\n');

	switch (choice) {
	case '1':
		while (!loaded) {
			printf("Enter file name:\n");

			for (int i = 0; i < 20; i++) {
				t = getchar();
				if (t == '\n') {
					filename[i] = '\0';
					break;
				}
				filename[i] = t;
			}


			loaded = sudfile(filename, board, diff);
		}
		break;
	case '2':

		while (*diff == 0) {

			printf("Set difficulty: (1-5)");

			choice = getchar();

			while ((getchar()) != '\n');

			*diff = choice - '0';

			if (*diff < 1 || *diff > 5) {
				*diff = 0;
			}
		}

		generate(board, *diff);

		break;
	}
	return;
}

void game(player_l* currentplayer) {
	int board[9][9] = { 0 };
	int history[81] = { 0 };
	int timestart = time(NULL);
	int timestop = 0;
	int savedtime = 0;
	int diff = 0;
	int choice;

	createboard(board, &diff);

	printf("For help enter h\n");

	while (1) {

		drawboard(board);

		choice = getchar();

		switch (choice) {
		case 'a':
			add(board, history);
			break;

		case 'b':
			while ((getchar()) != '\n');
			back(board, history);
			break;

		case 'e':
			emptycell(board, history);
			break;

		case 'c':
			while ((getchar()) != '\n');
			if (checkboard(board)) {

				timestop = time(NULL);

				endgame(currentplayer, timestop - timestart + savedtime, diff);
				return;
			}
			break;

		case 'm':
			while ((getchar()) != '\n');
			recallmoves(history);
			break;

		case 's':
			while ((getchar()) != '\n');

			timestop = time(NULL);
			savedtime = timestop - timestart + savedtime;

			save(board, history, time, diff);
			break;

		case 'l':
			while ((getchar()) != '\n');

			timestart = time(NULL);

			load(board, history, &savedtime, &diff);
			break;

		case 'r':
			while ((getchar()) != '\n');
			if (!solvesudoku(board)) {
				printf("Can't solve\n");
			}
			break;

		case 'h':
			while ((getchar()) != '\n');
			printhelp();
			break;

		case 'q':
			while ((getchar()) != '\n');
			printf("Exiting\n");
			return;
		}
	}
	return;
}

void add(int(*board)[9], int *history) {
	int row;
	int col;
	int val;

	row = getchar() - '0';
	if (row > 9 || row < 1) {
		printf("Wrong parameters\n");
		return;
	}
	col = getchar() - '0';
	if (col > 9 || col < 1) {
		printf("Wrong parameters\n");
		return;
	}
	val = getchar() - '0';
	if (val > 9 || val < 1) {
		printf("Wrong parameters\n");
		return;
	}

	while ((getchar()) != '\n');

	makechoice(board, row - 1, col - 1, val, history);
	printf("Choice made: row: %i, column: %i, value: %i.\n", row, col, val);
	return;
}

void recallmoves(int* history) {

	printf("Your moves: %i\n", history[0]);

	for (int i = 1; i <= history[0]; i++) {
		printf("row: %i, column: %i.\n", history[i] / 10, history[i] % 10);
	}

}

void emptycell(int(*board)[9], int *history) {

	int row;
	int col;
	bool removed = false;

	row = getchar() - '0';
	if (row > 9 || row < 1) {
		printf("Wrong parameters\n");
		return;
	}
	col = getchar() - '0';
	if (col > 9 || col < 1) {
		printf("Wrong parameters\n");
		return;
	}

	while ((getchar()) != '\n');

	removed = erase(board, row - 1, col - 1, history);

	if (removed) {
		printf("Erased: row: %i, column : %i.\n", row, col);
	}
	else {
		printf("You cant erase: row: %i, column : %i.\n", row, col);
	}

	return;
}

bool checkboard(int(*board)[9]) {

	bool check = true;

	check = check && checkempty(board);

	if (!check) {
		printf("There are still empty cells!\n");
	}

	for (int i = 0; i < 9; i++) {

		check = check && checkrow(board, i);
		if (!checkrow(board, i)) {
			printf("Repetiton in row: %i\n", i + 1);
		}

		check = check && checkcolumn(board, i);
		if (!checkcolumn(board, i)) {
			printf("Repetiton in column: %i\n", i + 1);
		}

		check = check && checksquare(board, i);
		if (!checksquare(board, i)) {
			printf("Repetiton in square: %i\n", i + 1);
		}

	}

	return check;
}

void load(int(*board)[9], int*history, int*time, int *diff) {

	char filename[21] = { '\0' };
	char t;

	printf("Enter file name: \n");

	for (int i = 0; i < 20; i++) {
		t = getchar();
		if (t == '\n') {
			break;
		}
		filename[i] = t;
	}

	if (!loadgame(board, history, time, filename, diff)) {
		printf("File not found");
	}
	else {
		printf("Loaded game:, %s\n", filename);
	}
	return;
}

void save(int(*board)[9], int* history, int time, int diff) {
	char filename[21] = { '\0' };
	char t;

	printf("Enter file name: \n");

	for (int i = 0; i < 20; i++) {
		t = getchar();
		if (t == '\n') {
			break;
		}
		filename[i] = t;
	}

	savegame(board, history, time, filename, diff);

	printf("Saved game:, %s\n", filename);
	return;
}

void endgame(player_l* currentplayer, int time, int diff) {

	game_l* temp = currentplayer->gamelist;
	game_l* curr = malloc(sizeof(game_l));

	curr->diff = diff;
	curr->time = time;
	curr->score = diff * 20000 / (time + 1);
	curr->next = NULL;

	if (temp == NULL) {
		currentplayer->gamelist = curr;
	}
	else {
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = curr;
	}

	printf("Congartulations, sudoku solved\nTime: %is\nScore: %i\n", curr->time, curr->score);

	currentplayer->gamecount += 1;
	currentplayer->totalscore += diff * 20000 / (time + 1);

}

void close(player_l*playerlist) {

	player_l* temp = playerlist;
	int playercount = 0;

	while (temp != NULL) {
		savegames(temp);

		temp = temp->next;
		playercount++;
	}

	saveplayers(playerlist, playercount);

	freelists(playerlist);

	return;
}


void printhelp(void) {

	printf("HELP:\nEnter corresponding letter along with parameteres - [x]\n");
	printf("a[x][y][z] - make choice in row [x], column [y] by placing [z] value\n");
	printf("b - erase last move\n");
	printf("e[x][y] - clear cell in row [x] and column [y]\n");
	printf("c - check sudoku, if its filled correctly ends the game\n");
	printf("s - save the game\n");
	printf("l - load the game\n");
	printf("r - computer will try to resolve sudoku\n");
	printf("q - quit the game\n");
	printf("note: replace [x] with desired value\n\n");

	return;
}

void printplayerlist(player_l* playerlist) {

	player_l* temp_p = playerlist;

	while (temp_p != NULL) {
		printf("%i. %s score: %i, %i games\n", temp_p->id, temp_p->name, temp_p->totalscore, temp_p->gamecount);
		temp_p = temp_p->next;
	}

	return;
}

void printgamelist(game_l* gamelist) {

	game_l* temp_p = gamelist;

	if (temp_p == NULL) {
		printf("You have no games\n");
	}

	while (temp_p != NULL) {
		printf("Difficulty: %i, time: %i, score: %i\n", temp_p->diff, temp_p->time, temp_p->score);
		temp_p = temp_p->next;
	}
	return;
}

void drawboard(int(*board)[9]) {

	printf("   ");

	for (int i = 0; i < 9; i++) {
		if ((i + 1) % 3 == 0) {
			printf("%i  ", i + 1);
		}
		else {
			printf("%i ", i + 1);
		}
	}
	printf("\n\n");

	for (int i = 0; i < 9; i++) {


		printf("%i  ", i + 1);

		for (int j = 0; j < 9; j++) {

			if (board[i][j] == 0) {
				if ((j + 1) % 3 == 0) {
					printf("   ");
				}
				else {
					printf("  ");
				}
			}
			else {
				if ((j + 1) % 3 == 0) {
					printf("%i  ", board[i][j]);
				}
				else {
					printf("%i ", board[i][j]);
				}
			}
		}

		printf("\n");

		if ((i + 1) % 3 == 0) {
			printf("\n");
		}

	}
	return;
}