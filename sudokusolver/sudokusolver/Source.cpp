#include "functionsheader.h"
#include <iostream>
#include <string>
#include "filesheader.h"
//#include <vld.h>

int main(int argc, char*argv[])
{
	if (argc != FORESEEN_ARGC)
	{
		cout << "Number of parameters is not correct." << endl;
		cout << HowToUse();
		system("pause");
		return 0;
	}

	Files *files;
	try
	{
		files = new Files(argv);
	}
	catch (exception & e)
	{
		cout << e.what() << endl;
		cout << HowToUse();
		system("pause");
		return 0;
	}

	vector<vector<int>> grid;
	try
	{
		grid = TakeGridFromFile(*files->input());
	}
		
	catch (exception& e)
	{
		*(files->output()) << e.what() << endl;
		*(files->output()) << HowToUse();
		delete files;
		return 0;
	}
	catch (...)
	{
		*(files->output()) << "Exception Occured." << endl;
		*(files->output()) << HowToUse();
		delete files;
		return 0;
	}

	SudokuDiagram diagram = SudokuDiagram(grid);
	if (diagram.SolveSudoku())
	{
		diagram.WriteToFile(*(files->output()));
	}
	else
	{
		*(files->output()) << "Input sudoku diagram is invalid.";
	}	
	delete files;
	return 1;
}