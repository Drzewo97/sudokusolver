# Project Title

sudokusolver allows user to solve valid sudoku diagram provided via text file.

# How it works...

Program works like this: When you provide correct path to file, f.e. sudoku.txt after switcher ‘-i’ it prints out solved Sudoku or proper error message in output file, which you provide like input file, but after switcher ‘-o’. Each character in input file should represent corresponding cell in diagram. To properly indicate value of cell put there a digit. If cell has no value put there zero or any character that is not a digit. Do not separate cells with anything. To move to next row of diagram use line feed.

Example of correct call of the program:

```
C:\sudokusolver\sudokusolver.exe –i C:\sudokusolver\sudoku.txt –o C:\sudokusolver\output.txt
```

# Comments

Program was written as Computer Programming Project in April, 2017.