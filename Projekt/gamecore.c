#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h>
#include "interface.h"

bool sudfile(char* filename, int(*board)[9], int* diff) {

	FILE* sfile = fopen(filename, "r");

	if (sfile == NULL) {
		printf("File not found\n");
		return false;
	}

	*diff = (int)getc(sfile) - '0';

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			board[i][j] = (int)getc(sfile) - '0';
		}
	}

	fclose(sfile);
	return true;
}

void generate(int(*board)[9], int diff) {

	int r;
	int c;
	int sq;
	int val;
	bool check;


	for (int i = 0; i < 45 - diff * 5; i++) {
		while (1) {
			check = true;
			r = (rand() + time(NULL) % 100) % 9 + 1;
			c = (rand() + time(NULL) % 100) % 9 + 1;
			val = (rand() + time(NULL) % 100) % 9 + 1;
			if (board[r][c] == 0) {
				board[r][c] = val;

				check = check && checkrow(board, r, 0);
				check = check && checkcolumn(board, c, 0);


				sq = (r - 1) / 3 + ((c - 1) / 3) * 3;

				check = check && checksquare(board, sq, 0);

				if (check == true) {
					break;
				}
				else {
					board[r][c] = 0;
				}
			}
		}


	}

	return;
}

void makechoice(int(*board)[9], int row, int column, int val, int* history) {

	if (board[row][column] != 0) {
		printf("Can't make that choice, (%i, %i) filled", row + 1, column + 1);
	}
	else {
		board[row][column] = val;
		history[0] += 1;
		history[history[0]] = (row + 1) * 10 + column + 1;
	}
}

void back(int(*board)[9], int*history) {
	if (history[0] != 0) {
		int row = history[history[0]] / 10;
		int column = history[history[0]] - row * 10;

		board[row - 1][column - 1] = 0;

		history[history[0]] = 0;

		history[0] -= 1;
	}
	else
		printf("History is empty\n");
}

bool erase(int(*board)[9], int row, int column, int*history) {

	bool removed = false;

	if (board[row][column] != 0) {
		for (int i = 1; i <= history[0]; i++) {

			if ((history[i] / 10) == row + 1 && (history[i] % 10) == column + 1) {
				history[i] = 0;
				board[row][column] = 0;
				for (int j = i + 1; j <= history[0]; j++) {
					history[j - 1] = history[j];
				}
				history[0] -= 1;
				removed = true;
				break;
			}
		}
	}
	return removed;
}

bool checkempty(int(*board)[9]) {

	bool check = true;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (board[i][j] == 0) {
				check = false;
			}
		}
	}

	return check;
}

bool checkrow(int(*board)[9], int row) {

	int temp;
	bool check = true;

	for (int i = 0; i < 9; i++) {
		temp = board[row][i];
		if (temp != 0) {
			for (int j = i + 1; j < 9; j++) {
				if (temp == board[row][j]) {
					check = false;
				}
			}
		}
	}
	return check;
}

bool checkcolumn(int(*board)[9], int column) {

	int temp;
	bool check = true;

	for (int i = 0; i < 9; i++) {
		temp = board[i][column];
		if (temp != 0) {
			for (int j = i + 1; j < 9; j++) {
				if (temp == board[j][column]) {
					check = false;
				}
			}
		}
	}
	return check;
}

bool checksquare(int(*board)[9], int square) {

	int temp;
	bool check = true;

	for (int i = 0; i < 9; i++)
	{
		temp = board[(square) / 3 * 3 + i / 3][(square) % 3 * 3 + i % 3];
		if (temp != 0) {
			for (int j = i + 1; j < 9; j++) {
				if (temp == board[(square) / 3 * 3 + j / 3][(square) % 3 * 3 + j % 3]) {
					check = false;
				}
			}
		}
	}

	return check;
}