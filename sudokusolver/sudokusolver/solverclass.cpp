#include "sudokudiagramheader.h"
#include <algorithm>
#include <iostream>

int CandidateCount(int candidate, vector<Cell*> cells);
int GetSubxFromx(int x, int size);
int GetSubyFromy(int y, int size);
bool IdenticalCandidates(Cell *cell1, Cell *cell2);
int SquareVectorIndex(int x, int y, int size);
bool DifferentButSame(tuple<Cell*, int, int> tupyl1, tuple<Cell*, int, int> tupyl2);

Solver::Solver(SudokuGrid *&sudokugrid)
{
	sudokugrid_ = sudokugrid;
	SetCellsQueue();
}

Solver::Solver()
{

}

Solver::~Solver()
{
}

bool Solver::SolveSudoku()
{
	bool flag = true;
	while (flag)
	{
		cellqueue_.SortQueue();

		//Naked single
		while (cellqueue_.CellsLeft() > 0 && cellqueue_.PeekCandidatesCount() == 1)
		{
			if(sudokugrid_->FillCell(cellqueue_.Pull()))
				cellqueue_.SortQueue();
		}
		if (cellqueue_.CellsLeft() == 0)
		{
			//o³ je
			return true;
		}
		if (cellqueue_.PeekCandidatesCount() < 1)
		{
			//o³ no³
			return false;
		}

		//Try logical methods, false if it didn't affect any cell
		flag = LogicalSolvingMethods();
	}

	return BruteForce();
}

bool Solver::BruteForce()
{
	Cell* cell = cellqueue_.Pull();
	vector<vector<int>> grid = sudokugrid_->Grid();
	for(int candidate : cell->Candidates())
	{
		grid[cell->XCoordinate()][cell->YCoordinate()] = candidate;

		SudokuDiagram auxsudoku = SudokuDiagram(grid);
		if (auxsudoku.SolveSudoku())
		{
			//this candidate is correct, auxsudoku is solved.
			sudokugrid_->FulfillGrid(auxsudoku.Grid());
			return true;
		}
	}
	//none of these candidates work, sudoku is invalid
	return false;
}

bool Solver::DetermineNakedSubsets()
{
	bool returner = false;
	for (Cell *cell : sudokugrid_->EmptyCells())
	{
		vector<Cell*> row = sudokugrid_->EmptyRowCells(cell);
		vector<Cell*> column = sudokugrid_->EmptyColumnCells(cell);
		vector<Cell*> square = sudokugrid_->EmptySquareCells(cell);

		vector<Cell*> block[3] = { row, column, square };

		for (int i = 0; i < 3; i++)
		{
			if (LookForNakedSubset(cell, block[i]))
				returner = true;
		}
	}
	return returner;
}

bool Solver::DeterminePointingSubsets()
{
	bool returner = false;
	for (Cell *cell : sudokugrid_->EmptyCells())
		if (LookForPointingSubset(cell))
			returner = true;

	return returner;
}

bool Solver::DetermineUniqueCandidates()
{
	bool returner = false;
	for(Cell *cell : sudokugrid_->EmptyCells())
		if (LookForUniqueCandidate(cell))
			returner = true;

	return returner;
}

bool Solver::LogicalSolvingMethods()
{
	if (DetermineUniqueCandidates() || DetermineNakedSubsets() || XYWings() || DeterminePointingSubsets())
		return true;

	return false;
}

//Naked Subset (for example naked pair) is a pair of cells in a block (column, row or square) that are the only cells
//that contain given candidates (for pair 2, for triple 3 etc.)
//It gives us information that these 2 candidates cannot be anywhere else in that block
bool Solver::LookForNakedSubset(Cell *cell, vector<Cell*>& emptycells)
{
	bool returner = false;

	vector<Cell*> changedcells;
	int cellscount = 0;
	for(Cell *emptycell : emptycells)
	{
		if (IdenticalCandidates(cell, emptycell))
			cellscount++;
		else
			changedcells.push_back(emptycell);
	}

	if (cellscount == cell->CandidatesCount())
		//delete candidates from changedcells
		for(Cell *changedcell : changedcells)
			for(int candidate : cell->Candidates())
				if (changedcell->RemoveCandidate(candidate))
					returner = true;

	return returner;
}

//Pointing Subset (for example pointing pair) is a pair of cells inside the same square that are the only cells
//to contain given candidate. Therefore that candidate cannot be anywhere else in column or row in which these cells align.
//If they don't align, it gives us no information.
//Not proud of this functu
bool Solver::LookForPointingSubset(Cell *pcell) const
{
	bool returner = false;
	vector<Cell*> vec;
	for (int candidate : sudokugrid_->SquareCandidates(pcell))
	{
		for(Cell *cell : sudokugrid_->EmptySquareCells(pcell))
		{
			if (cell->IsCandidate(candidate))
				vec.push_back(cell);
		}

		//vec has all cells with candidate

		//cells can align only if they fulfill this condition, right?
		if (vec.size() > 1 && vec.size() <= sqrt(sudokugrid_->Size()))
		{
			if (sudokugrid_->CellsAligned(vec))
			{
				if (vec[0]->XCoordinate() == vec[1]->XCoordinate())
				{
					vector<Cell*> column = sudokugrid_->EmptyColumnCells(vec[0]);

					
//TODO:func			//delete cell from column 
					for(Cell* cell : vec)
						column.erase(remove(column.begin(), column.end(), cell), column.end());//erase-remove wiki

					for(Cell * cell : column)
						if (cell->RemoveCandidate(candidate))
							returner = true;
				}
				else if (vec[0]->YCoordinate() == vec[1]->YCoordinate())
				{
					vector<Cell*> row = sudokugrid_->EmptyRowCells(vec[0]);

					//delete cell from row 
					for(Cell* cell : vec)
						row.erase(remove(row.begin(), row.end(), cell), row.end());//erase-remove wiki
					
					for(Cell * cell : row)			
						if (cell->RemoveCandidate(candidate))
							returner = true;
				}
			}
		}
		vec.clear();
	}
	return returner;
}

//When there is only one cell in a block with certain candidate, it has to be its value
bool Solver::LookForUniqueCandidate(Cell * cell) const
{
	vector<Cell*> row = sudokugrid_->EmptyRowCells(cell);//row zawiera te¿ interesuj¹c¹ nas komórke
	vector<Cell*> column = sudokugrid_->EmptyColumnCells(cell);
	vector<Cell*> square = sudokugrid_->EmptySquareCells(cell);

	vector<Cell*> block[3] = { row, column, square };

	for(int candidate : cell->Candidates())
	{
		for (int i = 0; i < 3; i++)
		{
			//Block contains cell
			if (CandidateCount(candidate, block[i]) == 1)
			{
				cell->SetOnlyCandidate(candidate);
				return true;
			}
		}
	}
	return false;
}

//XY Wing involves 3 different cells – each with exactly two candidates.
//In order for this to work, the cell in the middle must intersect both of the cells on its wings, 
//while the two wing cells may not intersect each other.
//Each wing must share exactly one candidate with middle cell, but that candidate cannot be same in both of wings.
//Moreover, second candidate, the one different from middle cell has to be common for wings.
//Then, we can eliminate common candidate of wings from common cells of wings.
//GG
bool Solver::LookForXYWing(Cell * cell)
{
	bool returner = false;

	//tuple<cell*, common candidate, different candidate>
	vector<tuple<Cell*, int, int>> CandidateLinkedCells = sudokugrid_->EmptyCandidateLinkedCells(cell);	
	//CandidateLinkedCells consists of linked cells with only ONE OF CANDIDATES of cell

	vector<tuple<Cell*, int, int>>::iterator main;
	for (main = CandidateLinkedCells.begin(); main != CandidateLinkedCells.end(); main++)
	{
		vector<tuple<Cell*, int, int>>::iterator moving;
		for (moving = main + 1; moving != CandidateLinkedCells.end(); moving++)
		{
			if (DifferentButSame(*main, *moving))
			{
				for(Cell *commoncell : sudokugrid_->EmptyCommonCells(get<0>(*main), get<0>(*moving)))
					if (commoncell->RemoveCandidate(get<2>(*main)))
						returner = true;
			}

		}
	}
	return returner;
}

void Solver::SetCellsQueue()
{
	cellqueue_.ResetQueue();

	for(Cell *cell : sudokugrid_->EmptyCells())
		cellqueue_.Push(cell);
}

bool Solver::XYWings()
{
	bool returner = false;
	for (Cell *cell : sudokugrid_->EmptyCells(2))
	{
		if (cell->CandidatesCount() == 2)//It may have updated cells in queue
			if (LookForXYWing(cell))
			{
				returner = true;
			}
	}
	return returner;
}