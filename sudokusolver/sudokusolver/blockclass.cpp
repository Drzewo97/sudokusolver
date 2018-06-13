#include "blockheader.h"


Block::Block(vector<int> values, int size)
{
	valuesincluded_.resize(size, false);

	for (vector<int>::iterator it = values.begin(); it != values.end(); it++)
		valuesincluded_[*it - 1] = true;
}

Block::~Block()
{
}

vector<int> Block::Candidates()
{
	vector<int> returner;
	int i = 1;
	for(bool val : valuesincluded_)
	{
		if (!val)
			returner.push_back(i);
		i++;
	}
	return returner;
}

deque<bool> Block::GetTruthtable()
{
	return valuesincluded_;
}

void Block::IncludeValue(int value)
{
	valuesincluded_[value] = true;
}