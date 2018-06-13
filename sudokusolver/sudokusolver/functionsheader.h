#pragma once
#include "sudokudiagramheader.h"
#include <fstream>

int CandidateCount(int candidate, vector<Cell*> cells);
int GetSubxFromx(int x, int size);
int GetSubyFromy(int y, int size);
int SquareVectorIndex(int x, int y, int size);
bool DifferentButSame(tuple<Cell*, int, int> cell1, tuple<Cell*, int, int> cell2);
void FilterCells(vector<Cell*> &cells, int candidatescount);
void DeleteDuplicates(vector<Cell*> &v);
bool IdenticalCandidates(Cell *cell1, Cell *cell2);
vector<vector<int>> ReadFile(ifstream &input);
bool IsPerfectSquare(int value);
void Transpose(vector<vector<int>>& grid);
void CheckGridCorrectness(vector<vector<int>> grid);
vector<vector<int>> TakeGridFromFile(ifstream &input);
string HowToUse();