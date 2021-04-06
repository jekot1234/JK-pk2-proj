#include"gamecore.h"
#ifndef AI_H
#define AI_H
char solvesudoku(int(*board)[9]);

char findempty(int(*board)[9], int *row, int *column);

char inrow(int(*board)[9], int *row, int val);

char incolumn(int(*board)[9], int *column, int val);

char insquare(int(*board)[9], int sqrow, int sqcolumn, int val);

char safe(int(*board)[9], int row, int column, int val);

#endif