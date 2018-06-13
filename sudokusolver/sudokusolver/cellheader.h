#pragma once
#include <vector>

using namespace std;

struct Cell
{
public:
	Cell(int x, int y);
	~Cell();

	int CandidatesCount();
	bool Filled();
	vector<int> Candidates();
	void InsertCandidate(int value);
	void InsertCandidates(vector<bool> candidates);
	void InsertValue(int value);
	bool IsCandidate(int value);
	bool RemoveCandidate(int notzerobasedvalue);
	void SetOnlyCandidate(int value);
	void SetValueBasedOnCandidates();
	int Value();
	int XCoordinate();
	int YCoordinate();
private:
	int x_;
	int y_;
	int candidatescount_;
	int value_;
	bool filled_;
	vector<bool> candidates_;

	void ResetCandidates();
};
