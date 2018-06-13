#pragma once
#include <vector>

using namespace std;

struct Cell;

class PriorityQueue
{
public:
	PriorityQueue();
	~PriorityQueue();

	void Push(Cell* cell);
	int CellsLeft();
	int PeekCandidatesCount();
	Cell* Pull();
	void ResetQueue();
	void SortQueue();

	struct CellCompare
	{
		bool operator()(Cell *cell1, Cell *cell2);
	};

private:
	vector<Cell*> cellqueue_;
};
