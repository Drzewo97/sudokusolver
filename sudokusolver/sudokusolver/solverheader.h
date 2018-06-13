#pragma once
#include "priorityqueueheader.h"

using namespace std;

class SudokuGrid;

class Solver
{
public:
	Solver(SudokuGrid *&sudokugrid);
	Solver();
	~Solver();

	bool SolveSudoku();
private:
	PriorityQueue cellqueue_;
	SudokuGrid *sudokugrid_;

	bool BruteForce();
	bool DetermineNakedSubsets();
	bool DeterminePointingSubsets();
	bool DetermineUniqueCandidates();
	bool LogicalSolvingMethods();
	bool LookForNakedSubset(Cell *cell, vector<Cell*>& emptycells);
	bool LookForPointingSubset(Cell *cell) const;
	bool LookForUniqueCandidate(Cell *cell) const;
	bool LookForXYWing(Cell *cell);
	void SetCellsQueue();
	bool XYWings();
};