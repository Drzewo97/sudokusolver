#include "priorityqueueheader.h"
#include "cellheader.h"
#include <vector>
#include <algorithm>

using namespace std;

PriorityQueue::PriorityQueue()
{
}

PriorityQueue::~PriorityQueue()
{
}

void PriorityQueue::Push(Cell * cell)
{
	cellqueue_.push_back(cell);
}

int PriorityQueue::CellsLeft()
{
	return cellqueue_.size();
}

int PriorityQueue::PeekCandidatesCount()
{
	return cellqueue_.back()->CandidatesCount();
}

Cell * PriorityQueue::Pull()
{
	Cell *returner = cellqueue_.back();
	cellqueue_.pop_back();
	return returner;
}

void PriorityQueue::ResetQueue()
{
	cellqueue_.clear();
}

void PriorityQueue::SortQueue()
{
	sort(cellqueue_.begin(), cellqueue_.end(), CellCompare());
}

bool PriorityQueue::CellCompare::operator() (Cell *cell1, Cell *cell2)
{
	if (cell2->CandidatesCount() < cell1->CandidatesCount())
		return true;

	return false;
}