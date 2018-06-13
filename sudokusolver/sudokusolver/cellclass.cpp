#include "cellheader.h"

//Initialize cell with zero-based coordinates
Cell::Cell(int x, int y)
{
	value_ = 0;
	filled_ = false;
	x_ = x;
	y_ = y;
}

Cell::~Cell()
{
}

int Cell::CandidatesCount()
{
	return candidatescount_;
}

bool Cell::Filled()
{
	return filled_;
}

vector<int> Cell::Candidates()
{
	vector<int> candidates;
	int i = 1;
	for(bool candidate : candidates_)
	{
		if (candidate)
			candidates.push_back(i);
		i++;
		//TODO:What if for doesn't go from beg to end?
	}
	return candidates;
}

void Cell::InsertCandidate(int notzerobasedvalue)
{
	if (!candidates_[notzerobasedvalue - 1])
	{
		candidates_[notzerobasedvalue - 1] = true;
		candidatescount_++;
	}
}

//Replace candidates_ with parameter
void Cell::InsertCandidates(vector<bool> candidates)
{
	candidates_ = candidates;
	int counter = 0;
	for (bool candidate : candidates_)
	{
		if (candidate)
			counter++;
	}
	candidatescount_ = counter;
}

//Set (confident) value to a cell
void Cell::InsertValue(int value)
{
	value_ = value;
	filled_ = true;
	candidatescount_ = 0;
}

bool Cell::IsCandidate(int notzerobasedvalue)
{
	return candidates_[notzerobasedvalue - 1];
}

bool Cell::RemoveCandidate(int notzerobasedvalue)
{
	if (candidates_[notzerobasedvalue - 1])
	{
		candidates_[notzerobasedvalue - 1] = false;
		candidatescount_--;
		return true;
	}
	return false;
}

void Cell::SetOnlyCandidate(int value)
{
	ResetCandidates();
	candidates_[value - 1] = true;
	candidatescount_ = 1;
}

//At this point candidates_ has only one valid candidate
void Cell::SetValueBasedOnCandidates()
{
	//TODO: if candidatescount == 1?
	int i = 0;
	for (bool candidate : candidates_)
	{
		if (candidate)
		{
			InsertValue(i + 1);
			break;
		}
		i++;
	}
}

int Cell::Value()
{
	return value_;
}

int Cell::XCoordinate()
{
	return x_;
}

int Cell::YCoordinate()
{
	return y_;
}

//Set candidates_ to vector of previous size, all false
void Cell::ResetCandidates()
{
	for (vector<bool>::iterator it = candidates_.begin(); it != candidates_.end(); it++)
		*it = false;

	candidatescount_ = 0;
}