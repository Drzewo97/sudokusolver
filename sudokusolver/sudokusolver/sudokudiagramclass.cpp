#include "sudokudiagramheader.h"

#define SUDOKU_SIZE 9

SudokuDiagram::SudokuDiagram(vector<vector<int>> grid)
{
	sudokugrid_ = new SudokuGrid(grid);
	solver_ = Solver(sudokugrid_);
}

//Run without input file. Hardcodediagram()
SudokuDiagram::SudokuDiagram()
{
	sudokugrid_ = new SudokuGrid();
	solver_ = Solver(sudokugrid_);
}

SudokuDiagram::~SudokuDiagram()
{
	delete sudokugrid_;
}

vector<vector<int>> SudokuDiagram::Grid()
{
	return sudokugrid_->Grid();
}

void SudokuDiagram::DisplaySudoku()
{
	sudokugrid_->DisplayGrid();
}

void SudokuDiagram::WriteToFile(ofstream & output)
{
	sudokugrid_->WriteToFile(output);
}

bool SudokuDiagram::SolveSudoku()
{
	return solver_.SolveSudoku();
}
