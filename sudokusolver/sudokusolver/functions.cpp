#include "functionsheader.h"
#include <algorithm>

//Returns number of candidate in vector cells
int CandidateCount(int candidate, vector<Cell*> cells)
{
	int count = 0;
	for(Cell *cell : cells)
		if (cell->IsCandidate(candidate))
			count++;

	return count;
}

//For better square recognition, squares have subx like cells in grid have x
//Returns square value of X
int GetSubxFromx(int x, int size)
{
	int sizesqrt = (int)sqrt(size);
	return (x - x%sizesqrt) / sizesqrt;
}

//For better square recognition, squares have suby like cells in grid have y
//Returns block-square value of Y
int GetSubyFromy(int y, int size)
{
	int sizesqrt = (int)sqrt(size);
	return (y - y%sizesqrt) / sizesqrt;
}

//Returns index of square in vector square based on how they are added in there
int SquareVectorIndex(int x, int y, int size)
{
	int subx = GetSubxFromx(x, size);
	int suby = GetSubyFromy(y, size);

	int sizesqrt = (int)sqrt(size);

	return (sizesqrt*subx) + suby;
}

//Returns true when first int is different, but second is same.
//Necesarry for XY Wing
bool DifferentButSame(tuple<Cell*, int, int> cell1, tuple<Cell*, int, int> cell2)
{
	if (get<1>(cell1) != get<1>(cell2) && get<2>(cell1) == get<2>(cell2))
		return true;

	return false;
}

//Remove cell with different candidatescount
void FilterCells(vector<Cell*> & cells, int candidatescount)
{
	vector<Cell*> aux = cells;
	cells.clear();
	//TODO: remove erase?
	for(Cell* cell : aux)
		if (cell->CandidatesCount() == candidatescount)
			cells.push_back(cell);
}

//Remove duplicates from vector
void DeleteDuplicates(vector<Cell*> & vec)
{
	sort(vec.begin(), vec.end());
	vec.erase(unique(vec.begin(), vec.end()), vec.end());
}

//Checks if cells have exact same candidates (same size and values)
bool IdenticalCandidates(Cell *cell1, Cell *cell2)
{
	vector<int> cell1candidates = cell1->Candidates();
	vector<int> cell2candidates = cell2->Candidates();

	//if distance is the same, then check values
	if (distance(cell1candidates.begin(), cell1candidates.end()) == distance(cell2candidates.begin(), cell2candidates.end())
		&& equal(cell1candidates.begin(), cell1candidates.end(), cell2candidates.begin()))
		return true;

	return false;
}

//Converts file to vector[rows][column] of chars from input file 
vector<vector<int>> ReadFile(ifstream & input)
{
	vector<vector<int>> rawvector;
	vector<int> aux;
	char c;
	while (input.get(c))
	{
		if (isdigit(c))
		{
			aux.push_back(c - '0');
			continue;
		}
		else if (c == '\n')
		{
			rawvector.push_back(aux); //new row
			aux.clear();
			continue;
		}
		//something that doesn't really interest us, so for sudoku it's blank
		aux.push_back(0);
	}

	if (aux.size() > 0)
		rawvector.push_back(aux);

	return rawvector;
}

bool IsPerfectSquare(int value)
{
	if (value < 0)
		return false;

	double root = round(sqrt(value));
	return value == root*root; 
	//root^2 is a perfect square because of round, value doesn't have to be
}

//Remember your algebra lessons? good.
//Works properly only for square matrix
void Transpose(vector<vector<int>> & grid)
{
	vector<vector<int>> aux;
	aux.resize(grid.size());

	for (int x = 0; x < (int)grid.size(); x++)
		for (int y = 0; y < (int)grid.size(); y++)
			aux[x].push_back(grid[y][x]);

	grid = aux;
}

void CheckGridCorrectness(vector<vector<int>> grid)
{
	if (!IsPerfectSquare(grid.size()) || grid.size() == 0)
		throw exception("Input sudoku is not correct. Size of sudoku isn't a perfect square.");

	for(vector<int> rowv : grid)
	{
		if (rowv.size() != grid.size())
			throw exception("Input sudoku is not correct. Grid isn't a square.");

		for(int value : rowv)
			if (value > grid.size())
				throw exception("Input sudoku is not correct. Value exceeds size of grid.");
	}
}

vector<vector<int>> TakeGridFromFile(ifstream &input)
{
	vector<vector<int>> grid = ReadFile(input);
	CheckGridCorrectness(grid);
	//Transpose transposes correctly only square, so we check before
	Transpose(grid);
	
	return grid;
}

string HowToUse()
{
	string buffer = "";

	buffer += "Program solves any sudoku, bot for now user can only insert diagram of size up to 9.";
	buffer += '\n';

	buffer += "Input file should be *.txt and each character should represent corresponding cell in diagram";
	buffer += '\n';

	buffer += "To properly indicate value of cell put there a digit. If cell has no value pu there 0 or any character that is not a digit.";
	buffer += '\n';

	buffer += "Don't seperate cells with anything.";
	buffer += '\n';

	buffer += "To move to next row of diagram use line feed.";
	buffer += '\n';

	buffer += "Let me remind you, that valid sudoku's size is a perfect square, and values doesnt exceed sudoku size.";
	buffer += '\n';

	buffer += "To properly indicate input file put '-i ' before path to the file";
	buffer += '\n';

	buffer += "To properly indicate output file put '-o ' before path to the file";
	buffer += '\n';

	return buffer;
}
