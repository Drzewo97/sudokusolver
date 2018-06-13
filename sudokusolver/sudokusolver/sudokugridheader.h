#pragma once
#include "cellheader.h"
#include "blockheader.h"
#include <set>
#include <tuple>

using namespace std;

class SudokuGrid
{
public:
	SudokuGrid();
	SudokuGrid(vector<vector<int>> grid);
	~SudokuGrid();
	vector<Cell*> operator[](int x);
	void DisplayGrid();

	bool FillCell(Cell *certaincell);
	vector<vector<int>> Grid();
	void FulfillGrid(vector<vector<int>> grid);

	bool CellsAligned(vector<Cell*>& cells);
	vector<tuple<Cell*, int, int>> EmptyCandidateLinkedCells(Cell* cell);
	vector<Cell*> EmptyCells();
	vector<Cell*> EmptyCells(int candidatescount);
	vector<Cell*> EmptyColumnCells(Cell *cell);
	vector<Cell*> EmptyColumnCells(Cell *cell, int candidatescount);
	set<Cell*> EmptyCommonCells(Cell *cell1, Cell *cell2);
	vector<Cell*> EmptyLinkedCells(Cell *cell);
	vector<Cell*> EmptyLinkedCells(Cell *cell, int candidatescount);
	vector<Cell*> EmptyRowCells(Cell *cell);
	vector<Cell*> EmptyRowCells(Cell *cell, int candidatescount);
	vector<Cell*> EmptySquareCells(Cell *cell);
	vector<Cell*> EmptySquareCells(Cell *cell, int candidatescount);
	vector<int> SquareCandidates(Cell *cell);

	int Size();

	void WriteToFile(ofstream & output);
private:
	int size_;
	vector<vector<Cell*>> sudokugrid_;
	deque<Block*> rows_;
	deque<Block*> columns_;
	deque<Block*> squares_;

	vector<Cell*> Cells();
	bool CellsLinked(Cell *cell1, Cell *cell2);
	vector<Cell*> ColumnCells(int x);
	vector<int> ColumnValues(int x);
	vector<Cell*> EmptyColumnCells(int x);
	vector<Cell*> EmptyLinkedCells(int x, int y);
	vector<Cell*> EmptyRowCells(int y);
	vector<Cell*> EmptySquareCells(int subx, int suby);
	vector<Cell*> RowCells(int y);
	vector<int> RowValues(int y);
	
	vector<int> SquareCandidates(int x, int y);
	vector<Cell*> SquareCells(int subx, int suby);
	vector<int> SquareValues(int subx, int suby);
	vector<bool> TruthtableForCell(int x, int y);

	void CreateGrid(int size);
	void SetCellsCandidates();
	void SetPossibilities();

	void HardcodeDiagram();
};