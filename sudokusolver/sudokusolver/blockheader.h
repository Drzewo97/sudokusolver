#pragma once
#include <vector>
#include <deque>

using namespace std;

struct Block
{
public:
	Block(vector<int> values, int size);
	~Block();

	vector<int> Candidates();
	deque<bool> GetTruthtable();
	void IncludeValue(int value);
private:
	deque<bool> valuesincluded_;
};

