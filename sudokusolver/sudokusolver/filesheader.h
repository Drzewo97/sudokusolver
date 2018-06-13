#pragma once
#include <iostream>
#include <fstream>

#define FORESEEN_ARGC 5

using namespace std;

class Files
{
public:
	Files(char * argv[]);
	Files();
	ifstream * input();
	ofstream * output();
	~Files();
private:
	ifstream * input_;
	ofstream * output_;

	int ParameterIndex(char * argv[], string switcher);
};

