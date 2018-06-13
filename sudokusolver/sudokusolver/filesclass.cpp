#include "filesheader.h"


Files::Files(char * argv[])
{
	//cout << argv[ParameterIndex(argv, "-o")] << endl << argv[ParameterIndex(argv, "-i")] << endl;

	output_ = new ofstream(argv[ParameterIndex(argv, "-o")]);
	input_ = new ifstream(argv[ParameterIndex(argv, "-i")]);

	if (input()->fail())
	{
		throw exception("Input file didn't load correctly.");
	}

	if (output()->fail())
	{
		throw exception("Output file didn't load correctly.");
	}
}

Files::Files()
{
}

ifstream * Files::input()
{
	return input_;
}

ofstream * Files::output()
{
	return output_;
}


Files::~Files()
{
	delete input_;
	delete output_;
}

//get index of parameter in argv given by switcher just before parameter
int Files::ParameterIndex(char * argv[], string switcher)
{
	//TODO: zamiast 5 define
	for (int i = 1; i < FORESEEN_ARGC - 1; i = i + 2)
	{
		if (argv[i] == switcher)
		{
			return i + 1;
		}
	}
	throw exception("Invalid Parameters.");
}
