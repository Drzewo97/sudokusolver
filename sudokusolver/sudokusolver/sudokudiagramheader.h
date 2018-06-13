#pragma once
#include <vector>
#include "sudokugridheader.h"
#include "solverheader.h"

using namespace std;

//Contains information about one sudoku diagram
class SudokuDiagram
{
public:
	SudokuDiagram(vector<vector<int>> grid);
	SudokuDiagram();
	~SudokuDiagram();

	vector<vector<int>> Grid();
	void DisplaySudoku();
	void WriteToFile(ofstream& output);
	bool SolveSudoku();
private:
	SudokuGrid *sudokugrid_;
	Solver solver_;
};

