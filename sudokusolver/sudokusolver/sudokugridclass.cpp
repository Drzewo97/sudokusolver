#include "sudokugridheader.h"
#include <iostream>
#include <fstream>
#include "functionsheader.h"

SudokuGrid::SudokuGrid()
{
	size_ = 9;
	CreateGrid(size_);
	HardcodeDiagram();

	SetPossibilities();
	SetCellsCandidates();
}

SudokuGrid::SudokuGrid(vector<vector<int>> grid)
{
 	size_ = grid.size();
	CreateGrid(Size());
	
	for (int x = 0; x < Size(); x++)
	{
		for (int y = 0; y < Size(); y++)
		{
			if (grid[x][y] != 0)
				sudokugrid_[x][y]->InsertValue(grid[x][y]);
		}
	}

	SetPossibilities();
	SetCellsCandidates();
}

SudokuGrid::~SudokuGrid()
{
	for (int x = 0; x < Size(); x++)
	{
		for (int y = 0; y < Size(); y++)
		{
			delete sudokugrid_[x][y];
		}
	}
	
	for (int i = 0; i < Size(); i++)
	{
		delete columns_[i];
		delete rows_[i];
		delete squares_[i];
	}

}

vector<Cell*> SudokuGrid::operator[](int x)
{
	return sudokugrid_[x];
}

void SudokuGrid::DisplayGrid()
{
	system("cls");
	for (int y = 0; y < Size(); y++)
	{
		for (int x = 0; x < Size(); x++)
		{
			if (sudokugrid_[x][y]->Filled())
				cout << "  " << sudokugrid_[x][y]->Value();
			else
				cout << "  -";
		}
		cout << endl;
	}
}

//Set value to certaincell. Only when cell->candidatescount == 1
bool SudokuGrid::FillCell(Cell * certaincell)
{
	bool returner = false;

	certaincell->SetValueBasedOnCandidates();

	/*
	rows_[certaincell->YCoordinate()]->IncludeValue(certaincell->Value() - 1);
	columns_[certaincell->XCoordinate()]->IncludeValue(certaincell->Value() - 1);
	squares_[SquareVectorIndex(certaincell->XCoordinate(), certaincell->YCoordinate(), Size())]->IncludeValue(certaincell->Value() - 1);
	*/

	for(Cell* cell : EmptyLinkedCells(certaincell))
		if (cell->RemoveCandidate(certaincell->Value()))
			returner = true;

	return returner;
}

vector<vector<int>> SudokuGrid::Grid()
{
	vector<vector<int>> returner;
	
	returner.resize(Size());

	for (int i = 0; i < Size(); i++)
	{
		returner[i].resize(Size(), 0);
	}

	for (int x = 0; x < Size(); x++)
	{
		for (int y = 0; y < Size(); y++)
		{
			if (sudokugrid_[x][y]->Filled())
				returner[x][y] = sudokugrid_[x][y]->Value();
		}
	}

	return returner;
}

void SudokuGrid::FulfillGrid(vector<vector<int>> grid)
{
	for(Cell* emptycell : EmptyCells())
	{
		emptycell->InsertValue(grid[emptycell->XCoordinate()][emptycell->YCoordinate()]);
	}
}

//Return vector of all cells in grid
vector<Cell*> SudokuGrid::Cells()
{
	vector<Cell*> cells;

	for(vector<Cell*> row : sudokugrid_)
		for(Cell *cell : row)
			cells.push_back(cell);

	return cells;
}

bool SudokuGrid::CellsLinked(Cell * cell1, Cell * cell2)
{
	//If cells are aligned or in same square
	if (cell1->XCoordinate() == cell2->XCoordinate() || cell1->YCoordinate() == cell2->YCoordinate() || 
		SquareVectorIndex(cell1->XCoordinate(), cell1->YCoordinate(), Size()) == SquareVectorIndex(cell2->XCoordinate(), cell2->YCoordinate(), Size()))
		return true;

	return false;
}

//Return vector of all cells in column
vector<Cell*> SudokuGrid::ColumnCells(int x)
{
	vector<Cell*> column;
	for (Cell * cell : sudokugrid_[x])
		column.push_back(cell);
	
	return column;
}

//Return vector of all values in column
vector<int> SudokuGrid::ColumnValues(int x)
{
	vector<int> columnvalues;
	for(Cell *cell : ColumnCells(x))
	{
		if (cell->Filled())
			columnvalues.push_back(cell->Value());
	}
	return columnvalues;
}

//Check if cells share X or Y
bool SudokuGrid::CellsAligned(vector<Cell*>& cells)
{
	bool returner = true;
	if (cells.size() == 2)
	{
			if (cells[0]->XCoordinate() != cells[1]->XCoordinate() && cells[0]->YCoordinate() != cells[1]->YCoordinate())
				returner = false;
	}
	else
	{
		for (int i = 1; i < (int)cells.size(); i++)
		{
			if (cells[0]->XCoordinate() != cells[1]->XCoordinate() && cells[0]->YCoordinate() != cells[1]->YCoordinate() || cells[0]->XCoordinate() != cells[2]->XCoordinate() && cells[0]->YCoordinate() != cells[2]->YCoordinate() || cells[2]->XCoordinate() != cells[1]->XCoordinate() && cells[2]->YCoordinate() != cells[1]->YCoordinate())
			{
				returner = false;
				break;
			}
		}
	}
	return returner;
}


//Returns vector of cells linked to cell that share only one candidate with cell

	 //tuple<Cell*, common candidate, different candidate>
vector<tuple<Cell*, int, int>> SudokuGrid::EmptyCandidateLinkedCells(Cell* cell)
{
	vector<tuple<Cell*, int, int>> returner;

	vector<int> cellcandi = cell->Candidates();
	pair<int, int> cellpair = {cellcandi[0], cellcandi[1]};

	vector<Cell*> linkedcells = EmptyLinkedCells(cell);
	FilterCells(linkedcells, 2);

	for(Cell *linkedcell : linkedcells)
	{
		vector<int> linkedcandi = linkedcell->Candidates();
		pair<int, int> auxcandi = { linkedcandi[0], linkedcandi[1] };

		//We have to know which one is common and which one is different

		if (cellpair.first == auxcandi.first)
		{
			if (cellpair.second != auxcandi.second)
			{
				returner.push_back(tuple<Cell*, int, int>(linkedcell, auxcandi.first, auxcandi.second));
				continue;
			}
		}
		if (cellpair.first == auxcandi.second)
		{
			if (cellpair.second != auxcandi.first)
			{
				returner.push_back(tuple<Cell*, int, int>(linkedcell, auxcandi.second, auxcandi.first));
				continue;
			}
		}
		if (cellpair.second == auxcandi.first)
		{
			if (cellpair.first != auxcandi.second)
			{
				returner.push_back(tuple<Cell*, int, int>(linkedcell, auxcandi.first, auxcandi.second));
				continue;
			}
		}
		if (cellpair.second == auxcandi.second)
		{
			if (cellpair.first != auxcandi.first)
			{
				returner.push_back(tuple<Cell*, int, int>(linkedcell, auxcandi.second, auxcandi.first));
				continue;
			}
		}
	}
	return returner;
}

//Return vector of all empty cells in grid
vector<Cell*> SudokuGrid::EmptyCells()
{
	vector<Cell*> emptycells;
	for(Cell* cell : Cells())
	{
		if (!cell->Filled())
			emptycells.push_back(cell);
	}
	return emptycells;
}

//Return vector of all empty cells with candidatescount in grid 
vector<Cell*> SudokuGrid::EmptyCells(int candidatescount)
{
	vector<Cell*> returner = EmptyCells();
	FilterCells(returner, 2);
	return returner;
}

//Return vector of all empty cells in column
vector<Cell*> SudokuGrid::EmptyColumnCells(Cell * cell)
{
	return EmptyColumnCells(cell->XCoordinate());
}

//Return vector of all empty cells with candidatescount in column
vector<Cell*> SudokuGrid::EmptyColumnCells(Cell * cell, int candidatescount)
{
	vector<Cell*> returner = EmptyColumnCells(cell);
	FilterCells(returner, candidatescount);
	return returner;
}

vector<Cell*> SudokuGrid::EmptyColumnCells(int x)
{
	vector<Cell*> returner;
	for(Cell* cell : ColumnCells(x))
	{
		if (!cell->Filled())
			returner.push_back(cell);
	}
	return returner;
}

//Return set of all empty eclls common for cell1 and cell2
set<Cell*> SudokuGrid::EmptyCommonCells(Cell * cell1, Cell * cell2)
{
	set<Cell*> commoncells;
	
	//linked cells of cell1 that intersect with cell2 range
	for(Cell *cell : EmptyLinkedCells(cell1))
	{
		if (CellsLinked(cell, cell2))
			commoncells.insert(cell);
	}

	//linked cells of cell2 that intersect with cell1 range
	for(Cell *cell : EmptyLinkedCells(cell2))
	{
		if (CellsLinked(cell, cell1))
			commoncells.insert(cell);
	}

	commoncells.erase(cell1);
	commoncells.erase(cell2);

	return commoncells;
}

//Return vector of all empty cells linked to cell
vector<Cell*> SudokuGrid::EmptyLinkedCells(Cell * cell)
{
	return EmptyLinkedCells(cell->XCoordinate(), cell->YCoordinate());
}

//Return vector of all empty cells linked to cell that have candidatescount
vector<Cell*> SudokuGrid::EmptyLinkedCells(Cell * cell, int candidatescount)
{
	vector<Cell*> returner = EmptyLinkedCells(cell);
	FilterCells(returner, candidatescount);
	return returner;
}

//Return vector of all empty cells in row
vector<Cell*> SudokuGrid::EmptyRowCells(Cell * cell)
{
	return EmptyRowCells(cell->YCoordinate());
}

//Return vector of all empty cells with candidatescount in row
vector<Cell*> SudokuGrid::EmptyRowCells(Cell * cell, int candidatescount)
{
	vector<Cell*> returner = EmptyRowCells(cell);
	FilterCells(returner, candidatescount);
	return returner;
}

//Return vector of all empty cells in square
vector<Cell*> SudokuGrid::EmptySquareCells(Cell * cell)
{
	return EmptySquareCells(GetSubxFromx(cell->XCoordinate(), Size()), GetSubyFromy(cell->YCoordinate(), Size()));
}

//Return vector of all empty cells in square with candidatescount
vector<Cell*> SudokuGrid::EmptySquareCells(Cell * cell, int candidatescount)
{
	vector<Cell*> returner = EmptySquareCells(cell);
	FilterCells(returner, candidatescount);
	return returner;
}

//Return vector of all candidates in square
vector<int> SudokuGrid::SquareCandidates(Cell * cell)
{
	return SquareCandidates(cell->XCoordinate(), cell->YCoordinate());
}

vector<Cell*> SudokuGrid::EmptyLinkedCells(int x, int y)
{
	vector<Cell*> row = EmptyRowCells(y);
	vector<Cell*> column = EmptyColumnCells(x);
	vector<Cell*> square = EmptySquareCells(GetSubxFromx(x, size_), GetSubyFromy(y, size_));

	vector<Cell*> cells;
	cells.insert(cells.end(), row.begin(), row.end());
	cells.insert(cells.end(), column.begin(), column.end());
	cells.insert(cells.end(), square.begin(), square.end());

	DeleteDuplicates(cells);

	return cells;
}

vector<Cell*> SudokuGrid::EmptyRowCells(int y)
{
	vector<Cell*> returner;

	for(Cell* cell : RowCells(y))
	{
		if (!cell->Filled())
			returner.push_back(cell);
	}
	return returner;
}

vector<Cell*> SudokuGrid::EmptySquareCells(int subx, int suby)
{
	vector<Cell*> returner;

	for(Cell* cell : SquareCells(subx, suby))
	{
		if (!cell->Filled())
			returner.push_back(cell);
	}
	return returner;
}

vector<Cell*> SudokuGrid::RowCells(int y)
{
	vector<Cell*> row;
	for (int x = 0; x < size_; x++)
	{
		row.push_back(sudokugrid_[x][y]);
	}

	return row;
}

vector<int> SudokuGrid::RowValues(int y)
{
	vector<int> rowvalues;

	for(Cell *cell : RowCells(y))
	{
		if (cell->Filled())
			rowvalues.push_back(cell->Value());
	}

	return rowvalues;
}

vector<int> SudokuGrid::SquareCandidates(int x, int y)
{
	return squares_[SquareVectorIndex(x, y, Size())]->Candidates();
}

vector<Cell*> SudokuGrid::SquareCells(int subx, int suby)
{
	vector<Cell*> square;
	int sizesqrt = (int)sqrt(size_);

	for (int i = 0; i < sizesqrt; i++)
	{
		for (int j = 0; j < sizesqrt; j++)
		{
			square.push_back(sudokugrid_[(sizesqrt*subx) + i][(sizesqrt*suby) + j]);
		}
	}
	return square;
}

vector<int> SudokuGrid::SquareValues(int subx, int suby)
{
	vector<int> squarevalues;

	for(Cell *cell : SquareCells(subx, suby))
	{
		if (cell->Filled())
			squarevalues.push_back(cell->Value());
	}
	return squarevalues;
}

//Generate and return candidates_ for cell
vector<bool> SudokuGrid::TruthtableForCell(int x, int y)
{
	deque<bool> row = rows_[y]->GetTruthtable();
	deque<bool> column = columns_[x]->GetTruthtable();
	deque<bool> square = squares_[SquareVectorIndex(x, y, Size())]->GetTruthtable();

	vector<bool> returner;
	returner.resize(Size(), false);

	for (size_t i = 0; i < row.size(); i++)
	{
		if (!row[i] && !column[i] && !square[i])
			returner[i] = true;
	}

	return returner;
}

int SudokuGrid::Size()
{
	return size_;
}

//Pour sudoku grid into a file
void SudokuGrid::WriteToFile(ofstream & output)
{
	for (int y = 0; y < Size(); y++)
	{
		for (int x = 0; x < Size(); x++)
		{
			if (sudokugrid_[x][y]->Filled())
				output << "  " << sudokugrid_[x][y]->Value();
			else
				output << "  -";
		}
		output << endl;
	}
}

//Initialize cells for grid of size
void SudokuGrid::CreateGrid(int size)
{
	sudokugrid_.resize(size);

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			sudokugrid_[x].push_back(new Cell(x, y));
		}
	}
	size_ = size;
}

//Give each cell its candidates
void SudokuGrid::SetCellsCandidates()
{
	for (int x = 0; x < size_; x++)
	{
		for (int y = 0; y < size_; y++)
		{
			if (!sudokugrid_[x][y]->Filled())
				sudokugrid_[x][y]->InsertCandidates(TruthtableForCell(x, y));
		}
	}
}

//Give values to block
void SudokuGrid::SetPossibilities()
{
	for (int i = 0; i < Size(); i++)
	{
		rows_.push_back(new Block(RowValues(i), Size()));
		columns_.push_back(new Block(ColumnValues(i), Size()));
	}

	int sizesqrt = (int)sqrt(Size());
	for (int i = 0; i < sizesqrt; i++)
	{
		for (int j = 0; j < sizesqrt; j++)
			squares_.push_back(new Block(SquareValues(i, j), Size()));
	}
}

void SudokuGrid::HardcodeDiagram()
{
	//template
	/*sudokugrid_[0][0]->InsertValue();
	sudokugrid_[0][1]->InsertValue();
	sudokugrid_[0][2]->InsertValue();
	sudokugrid_[0][3]->InsertValue();
	sudokugrid_[0][4]->InsertValue();
	sudokugrid_[0][5]->InsertValue();
	sudokugrid_[0][6]->InsertValue();
	sudokugrid_[0][7]->InsertValue();
	sudokugrid_[0][8]->InsertValue();

	sudokugrid_[1][0]->InsertValue();
	sudokugrid_[1][1]->InsertValue();
	sudokugrid_[1][2]->InsertValue();
	sudokugrid_[1][3]->InsertValue();
	sudokugrid_[1][4]->InsertValue();
	sudokugrid_[1][5]->InsertValue();
	sudokugrid_[1][6]->InsertValue();
	sudokugrid_[1][7]->InsertValue();
	sudokugrid_[1][8]->InsertValue();

	sudokugrid_[2][0]->InsertValue();
	sudokugrid_[2][1]->InsertValue();
	sudokugrid_[2][2]->InsertValue();
	sudokugrid_[2][3]->InsertValue();
	sudokugrid_[2][4]->InsertValue();
	sudokugrid_[2][5]->InsertValue();
	sudokugrid_[2][6]->InsertValue();
	sudokugrid_[2][7]->InsertValue();
	sudokugrid_[2][8]->InsertValue();

	sudokugrid_[3][0]->InsertValue();
	sudokugrid_[3][1]->InsertValue();
	sudokugrid_[3][2]->InsertValue();
	sudokugrid_[3][3]->InsertValue();
	sudokugrid_[3][4]->InsertValue();
	sudokugrid_[3][5]->InsertValue();
	sudokugrid_[3][6]->InsertValue();
	sudokugrid_[3][7]->InsertValue();
	sudokugrid_[3][8]->InsertValue();

	sudokugrid_[4][0]->InsertValue();
	sudokugrid_[4][1]->InsertValue();
	sudokugrid_[4][2]->InsertValue();
	sudokugrid_[4][3]->InsertValue();
	sudokugrid_[4][4]->InsertValue();
	sudokugrid_[4][5]->InsertValue();
	sudokugrid_[4][6]->InsertValue();
	sudokugrid_[4][7]->InsertValue();
	sudokugrid_[4][8]->InsertValue();

	sudokugrid_[5][0]->InsertValue();
	sudokugrid_[5][1]->InsertValue();
	sudokugrid_[5][2]->InsertValue();
	sudokugrid_[5][3]->InsertValue();
	sudokugrid_[5][4]->InsertValue();
	sudokugrid_[5][5]->InsertValue();
	sudokugrid_[5][6]->InsertValue();
	sudokugrid_[5][7]->InsertValue();
	sudokugrid_[5][8]->InsertValue();

	sudokugrid_[6][0]->InsertValue();
	sudokugrid_[6][1]->InsertValue();
	sudokugrid_[6][2]->InsertValue();
	sudokugrid_[6][3]->InsertValue();
	sudokugrid_[6][4]->InsertValue();
	sudokugrid_[6][5]->InsertValue();
	sudokugrid_[6][6]->InsertValue();
	sudokugrid_[6][7]->InsertValue();
	sudokugrid_[6][8]->InsertValue();

	sudokugrid_[7][0]->InsertValue();
	sudokugrid_[7][1]->InsertValue();
	sudokugrid_[7][2]->InsertValue();
	sudokugrid_[7][3]->InsertValue();
	sudokugrid_[7][4]->InsertValue();
	sudokugrid_[7][5]->InsertValue();
	sudokugrid_[7][6]->InsertValue();
	sudokugrid_[7][7]->InsertValue();
	sudokugrid_[7][8]->InsertValue();

	sudokugrid_[8][0]->InsertValue();
	sudokugrid_[8][1]->InsertValue();
	sudokugrid_[8][2]->InsertValue();
	sudokugrid_[8][3]->InsertValue();
	sudokugrid_[8][4]->InsertValue();
	sudokugrid_[8][5]->InsertValue();
	sudokugrid_[8][6]->InsertValue();
	sudokugrid_[8][7]->InsertValue();
	sudokugrid_[8][8]->InsertValue();
	*/

	//sudoku #1 medium

	
	/*sudokugrid_[3][0]->InsertValue(5);
	sudokugrid_[4][0]->InsertValue(1);
	sudokugrid_[6][0]->InsertValue(6);
	sudokugrid_[8][0]->InsertValue(8);

	sudokugrid_[2][1]->InsertValue(8);
	sudokugrid_[8][1]->InsertValue(9);

	sudokugrid_[1][2]->InsertValue(1);
	sudokugrid_[2][2]->InsertValue(2);
	sudokugrid_[3][2]->InsertValue(6);
	sudokugrid_[7][2]->InsertValue(4);

	sudokugrid_[1][3]->InsertValue(6);
	sudokugrid_[3][3]->InsertValue(8);
	sudokugrid_[8][3]->InsertValue(3);

	sudokugrid_[1][4]->InsertValue(9);
	sudokugrid_[2][4]->InsertValue(4);
	sudokugrid_[5][4]->InsertValue(5);
	sudokugrid_[7][4]->InsertValue(2);

	sudokugrid_[2][5]->InsertValue(7);
	sudokugrid_[8][5]->InsertValue(5);

	sudokugrid_[0][6]->InsertValue(9);
	sudokugrid_[3][6]->InsertValue(4);
	sudokugrid_[5][6]->InsertValue(7);

	sudokugrid_[0][7]->InsertValue(1);
	sudokugrid_[2][7]->InsertValue(6);
	sudokugrid_[3][7]->InsertValue(2);
	sudokugrid_[4][7]->InsertValue(3);

	sudokugrid_[0][8]->InsertValue(4);
	sudokugrid_[4][8]->InsertValue(9);
	sudokugrid_[6][8]->InsertValue(3);
	sudokugrid_[7][8]->InsertValue(7);*/
	

	//sudoku #2 hard

	/*sudokugrid_[0][2]->InsertValue(6);
	sudokugrid_[0][3]->InsertValue(4);
	sudokugrid_[0][4]->InsertValue(9);

	sudokugrid_[1][2]->InsertValue(4);
	sudokugrid_[1][5]->InsertValue(5);

	sudokugrid_[2][0]->InsertValue(5);
	sudokugrid_[2][1]->InsertValue(8);
	sudokugrid_[2][6]->InsertValue(2);

	sudokugrid_[3][0]->InsertValue(2);

	sudokugrid_[4][0]->InsertValue(9);
	sudokugrid_[4][2]->InsertValue(8);
	sudokugrid_[4][4]->InsertValue(5);
	sudokugrid_[4][8]->InsertValue(6);

	sudokugrid_[5][3]->InsertValue(6);

	sudokugrid_[6][4]->InsertValue(4);
	sudokugrid_[6][5]->InsertValue(1);
	sudokugrid_[6][7]->InsertValue(7);
	sudokugrid_[6][8]->InsertValue(2);

	sudokugrid_[7][1]->InsertValue(1);
	sudokugrid_[7][2]->InsertValue(7);
	sudokugrid_[7][3]->InsertValue(3);
	sudokugrid_[7][7]->InsertValue(5);

	sudokugrid_[8][4]->InsertValue(6);
	sudokugrid_[8][5]->InsertValue(9);
	sudokugrid_[8][6]->InsertValue(3);
	sudokugrid_[8][7]->InsertValue(1);
	sudokugrid_[8][8]->InsertValue(8);
	*/

	//hard sudoku

	/*sudokugrid_[1][0]->InsertValue(1);
	sudokugrid_[2][0]->InsertValue(4);
	sudokugrid_[5][0]->InsertValue(3);
	sudokugrid_[6][0]->InsertValue(6);

	sudokugrid_[0][1]->InsertValue(5);
	sudokugrid_[3][1]->InsertValue(6);

	sudokugrid_[2][2]->InsertValue(7);
	sudokugrid_[3][2]->InsertValue(1);
	sudokugrid_[4][2]->InsertValue(9);
	sudokugrid_[6][2]->InsertValue(3);

	sudokugrid_[0][3]->InsertValue(8);
	sudokugrid_[3][3]->InsertValue(4);
	sudokugrid_[6][3]->InsertValue(2);

	sudokugrid_[1][4]->InsertValue(6);
	sudokugrid_[3][4]->InsertValue(2);
	sudokugrid_[5][4]->InsertValue(9);
	sudokugrid_[7][4]->InsertValue(3);

	sudokugrid_[2][5]->InsertValue(1);
	sudokugrid_[5][5]->InsertValue(6);
	sudokugrid_[8][5]->InsertValue(7);

	sudokugrid_[2][6]->InsertValue(6);
	sudokugrid_[4][6]->InsertValue(4);
	sudokugrid_[5][6]->InsertValue(5);
	sudokugrid_[6][6]->InsertValue(7);

	sudokugrid_[5][7]->InsertValue(8);
	sudokugrid_[8][7]->InsertValue(6);

	sudokugrid_[2][8]->InsertValue(9);
	sudokugrid_[3][8]->InsertValue(3);
	sudokugrid_[6][8]->InsertValue(1);
	sudokugrid_[7][8]->InsertValue(8);*/

	//sudoku #3 (it cannot be logically solved)

	
	/*sudokugrid_[0][1]->InsertValue(2);
	sudokugrid_[0][3]->InsertValue(3);
	sudokugrid_[0][7]->InsertValue(1);

	sudokugrid_[1][1]->InsertValue(9);
	sudokugrid_[1][2]->InsertValue(3);
	sudokugrid_[1][6]->InsertValue(7);
	sudokugrid_[1][8]->InsertValue(5);

	sudokugrid_[2][0]->InsertValue(4);
	sudokugrid_[2][1]->InsertValue(1);
	sudokugrid_[2][3]->InsertValue(7);
	sudokugrid_[2][4]->InsertValue(2);
	sudokugrid_[2][7]->InsertValue(9);

	sudokugrid_[3][6]->InsertValue(2);

	sudokugrid_[4][1]->InsertValue(6);

	sudokugrid_[5][0]->InsertValue(5);
	sudokugrid_[5][3]->InsertValue(6);
	sudokugrid_[5][5]->InsertValue(4);
	sudokugrid_[5][7]->InsertValue(3);

	sudokugrid_[6][3]->InsertValue(1);
	sudokugrid_[6][8]->InsertValue(4);

	sudokugrid_[7][4]->InsertValue(6);
	sudokugrid_[7][6]->InsertValue(9);

	sudokugrid_[8][1]->InsertValue(3);
	sudokugrid_[8][4]->InsertValue(7);
	sudokugrid_[8][7]->InsertValue(6);
	sudokugrid_[8][8]->InsertValue(2);*/

	//naked triple

	/*sudokugrid_[0][2]->InsertValue(5);
	sudokugrid_[0][3]->InsertValue(1);
	sudokugrid_[0][5]->InsertValue(2);

	sudokugrid_[1][2]->InsertValue(6);
	sudokugrid_[1][5]->InsertValue(7);
	sudokugrid_[1][6]->InsertValue(4);

	sudokugrid_[2][1]->InsertValue(1);
	sudokugrid_[2][4]->InsertValue(4);
	sudokugrid_[2][7]->InsertValue(2);

	sudokugrid_[3][1]->InsertValue(9);
	sudokugrid_[3][2]->InsertValue(3);
	sudokugrid_[3][3]->InsertValue(6);

	sudokugrid_[4][2]->InsertValue(1);
	sudokugrid_[4][6]->InsertValue(6);

	sudokugrid_[5][5]->InsertValue(4);
	sudokugrid_[5][6]->InsertValue(8);
	sudokugrid_[5][7]->InsertValue(5);

	sudokugrid_[6][1]->InsertValue(5);
	sudokugrid_[6][4]->InsertValue(7);
	sudokugrid_[6][7]->InsertValue(9);

	sudokugrid_[7][2]->InsertValue(9);
	sudokugrid_[7][3]->InsertValue(2);
	sudokugrid_[7][6]->InsertValue(3);

	sudokugrid_[8][3]->InsertValue(8);
	sudokugrid_[8][5]->InsertValue(3);
	sudokugrid_[8][6]->InsertValue(5);*/

	//solved until here

	//hidden triple

	//sudokugrid_[0][0]->InsertValue(3);
	//sudokugrid_[0][1]->InsertValue(9);
	//sudokugrid_[0][2]->InsertValue(6);
	//sudokugrid_[0][3]->InsertValue(2);
	//sudokugrid_[0][4]->InsertValue(5);
	//
	//sudokugrid_[1][1]->InsertValue(7);
	//
	//sudokugrid_[2][5]->InsertValue(8);
	//
	//sudokugrid_[3][2]->InsertValue(5);
	//sudokugrid_[3][4]->InsertValue(6);
	//sudokugrid_[3][6]->InsertValue(4);
	//
	//sudokugrid_[4][1]->InsertValue(1);
	//sudokugrid_[4][2]->InsertValue(8);
	//sudokugrid_[4][4]->InsertValue(2);
	//sudokugrid_[4][6]->InsertValue(3);
	//sudokugrid_[4][7]->InsertValue(9);
	//
	//sudokugrid_[5][2]->InsertValue(3);
	//sudokugrid_[5][4]->InsertValue(1);
	//sudokugrid_[5][6]->InsertValue(5);
	//
	//sudokugrid_[6][3]->InsertValue(9);
	//
	//sudokugrid_[7][7]->InsertValue(5);
	//
	//sudokugrid_[8][4]->InsertValue(3);
	//sudokugrid_[8][5]->InsertValue(5);
	//sudokugrid_[8][6]->InsertValue(2);
	//sudokugrid_[8][7]->InsertValue(6);
	//sudokugrid_[8][8]->InsertValue(1);

	//sudoku

	//sudokugrid_[0][0]->InsertValue(8);
	//sudokugrid_[0][1]->InsertValue(9);
	//sudokugrid_[0][7]->InsertValue(1);

	//sudokugrid_[1][0]->InsertValue(6);
	//sudokugrid_[1][7]->InsertValue(7);
	//sudokugrid_[1][8]->InsertValue(3);

	//sudokugrid_[2][0]->InsertValue(5);
	//sudokugrid_[2][2]->InsertValue(1);
	//sudokugrid_[2][3]->InsertValue(4);
	//sudokugrid_[2][7]->InsertValue(8);

	//sudokugrid_[3][0]->InsertValue(4);
	//sudokugrid_[3][4]->InsertValue(5);

	//sudokugrid_[4][4]->InsertValue(7);

	//sudokugrid_[5][2]->InsertValue(9);
	//sudokugrid_[5][5]->InsertValue(6);
	//sudokugrid_[5][7]->InsertValue(3);

	//sudokugrid_[6][2]->InsertValue(8);
	//sudokugrid_[6][3]->InsertValue(6);
	//sudokugrid_[6][5]->InsertValue(3);
	//sudokugrid_[6][6]->InsertValue(7);
	//sudokugrid_[6][7]->InsertValue(9);

	//sudokugrid_[7][1]->InsertValue(4);
	//sudokugrid_[7][2]->InsertValue(6);

	//sudokugrid_[8][3]->InsertValue(8);
	//sudokugrid_[8][4]->InsertValue(1);

//hardest sudoku

//sudokugrid_[0][0]->InsertValue(8);
//
//sudokugrid_[1][2]->InsertValue(7);
//sudokugrid_[1][3]->InsertValue(5);
//sudokugrid_[1][8]->InsertValue(9);
//
//sudokugrid_[2][1]->InsertValue(3);
//sudokugrid_[2][6]->InsertValue(1);
//sudokugrid_[2][7]->InsertValue(8);
//
//sudokugrid_[3][1]->InsertValue(6);
//sudokugrid_[3][5]->InsertValue(1);
//sudokugrid_[3][7]->InsertValue(5);
//
//sudokugrid_[4][2]->InsertValue(9);
//sudokugrid_[4][4]->InsertValue(4);
//
//sudokugrid_[5][3]->InsertValue(7);
//sudokugrid_[5][4]->InsertValue(5);
//
//sudokugrid_[6][2]->InsertValue(2);
//sudokugrid_[6][4]->InsertValue(7);
//sudokugrid_[6][8]->InsertValue(4);
//
//sudokugrid_[7][5]->InsertValue(3);
//sudokugrid_[7][6]->InsertValue(6);
//sudokugrid_[7][7]->InsertValue(1);
//
//sudokugrid_[8][6]->InsertValue(8);

//to broke brute force algorithm

//sudokugrid_[0][6]->InsertValue(5);
//
//sudokugrid_[1][5]->InsertValue(9);
//
//sudokugrid_[2][2]->InsertValue(1);
//sudokugrid_[2][4]->InsertValue(4);
//sudokugrid_[2][7]->InsertValue(2);
//
//sudokugrid_[3][3]->InsertValue(5);
//
//sudokugrid_[4][2]->InsertValue(2);
//sudokugrid_[4][7]->InsertValue(1);
//sudokugrid_[4][8]->InsertValue(4);
//
//sudokugrid_[5][1]->InsertValue(3);
//sudokugrid_[5][3]->InsertValue(7);
//
//sudokugrid_[6][4]->InsertValue(1);
//
//sudokugrid_[7][1]->InsertValue(8);
//sudokugrid_[7][6]->InsertValue(7);
//
//sudokugrid_[8][1]->InsertValue(5);
//sudokugrid_[8][6]->InsertValue(3);
//sudokugrid_[8][8]->InsertValue(9);

sudokugrid_[0][0]->InsertValue(1);
sudokugrid_[0][5]->InsertValue(6);
sudokugrid_[0][6]->InsertValue(3);

sudokugrid_[1][1]->InsertValue(3);
sudokugrid_[1][4]->InsertValue(1);
sudokugrid_[1][7]->InsertValue(4);

sudokugrid_[2][2]->InsertValue(9);
sudokugrid_[2][3]->InsertValue(5);
sudokugrid_[2][8]->InsertValue(7);

sudokugrid_[3][2]->InsertValue(6);
sudokugrid_[3][3]->InsertValue(3);

sudokugrid_[4][1]->InsertValue(2);
sudokugrid_[4][4]->InsertValue(8);

sudokugrid_[5][0]->InsertValue(7);
sudokugrid_[5][5]->InsertValue(4);

sudokugrid_[6][2]->InsertValue(5);
sudokugrid_[6][3]->InsertValue(9);
sudokugrid_[6][8]->InsertValue(3);

sudokugrid_[7][0]->InsertValue(9);
sudokugrid_[7][6]->InsertValue(1);

sudokugrid_[8][1]->InsertValue(8);
sudokugrid_[8][4]->InsertValue(2);
sudokugrid_[8][7]->InsertValue(7);

}