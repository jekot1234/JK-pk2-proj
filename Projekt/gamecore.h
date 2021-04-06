#include<stdbool.h>
#ifndef GAMECORE_H
#define GAMECORE_H

void generate(int(*board)[9], int diff);

bool sudfile(char* filename, int(*board)[9], int* diff);

void makechoice(int(*board)[9], int row, int column, int val, int* history);

bool erase(int(*board)[9], int row, int column, int*history);

void back(int(*board)[9], int*history);

bool checkempty(int(*board)[9]);

bool checkrow(int(*board)[9], int row);

bool checkcolumn(int(*board)[9], int column);

bool checksquare(int(*board)[9], int square);

#endif // !GAMECORE_H
