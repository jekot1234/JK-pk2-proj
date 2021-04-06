#include"gamecore.h"
#include"interface.h"
#include"AI.h"

char solvesudoku(int(*board)[9]) {

	int row, column;

	if (findempty(board, &row, &column) == 0) {
		return 1;
	}

	for (int val = 1; val <= 9; val++)
	{
		if (safe(board, row, column, val))
		{
			board[row][column] = val;

			if (solvesudoku(board))
				return 1;

			board[row][column] = 0;
		}
	}
	return 0;


}

char findempty(int(*board)[9], int *row, int *column) {

	for (*row = 0; *row < 9; (*row)++) {
		for (*column = 0; *column < 9; (*column)++) {
			if (board[*row][*column] == 0) {
				return 1;
			}
		}
	}
	return 0;
}

char inrow(int(*board)[9], int *row, int val) {

	for (int i = 0; i < 9; i++) {
		if (board[*row][i] == val) {
			return 1;
		}
	}

	return 0;
}

char incolumn(int(*board)[9], int *column, int val) {

	for (int i = 0; i < 9; i++) {
		if (board[i][*column] == val) {
			return 1;
		}
	}

	return 0;
}
char insquare(int(*board)[9], int sqrow, int sqcolumn, int val) {
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			if (board[row + sqrow][col + sqcolumn] == val)
				return 1;
	return 0;
}

char safe(int(*board)[9], int row, int column, int val) {

	return inrow(board, &row, val) == 0 && incolumn(board, &column, val) == 0 && insquare(board, row - row % 3, column - column % 3, val) == 0 && board[row][column] == 0;

}
