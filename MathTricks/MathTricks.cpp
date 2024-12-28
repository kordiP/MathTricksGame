#include <cmath>
#include <iomanip>
#include <iostream> // console io
#include <random>
#include <windows.h> // console colors

using namespace std;

const int FieldSizeMin = 4;
const int FieldSizeMax = 14;
int sizeRows;
int sizeColumns;

const int OperationsCount = 4;
const char OperationSymbols[] = { '+', '-', '*', '/' };

const char CellSeperator = '_';
const char CellNotSteppedOn = 'N';
const char CellSteppedOnByA = 'A';
const char CellSteppedOnByB = 'B';

const int TextWhite = 15;

enum StartMenuOptions;
struct Cell;

Cell** grid = nullptr;

void showStartMenu();
void initializeGrid(int rows, int cols);
void deleteGrid(int rows);
void generateGameSeed();
Cell generateCell(char symbol = '=', int value = -1);
void getFieldSize(int& rows, int& columns);
void clearConsole();
void clearInputBuffer();
void printGrid();
int generateTrulyRandomNumber(int lowLimit, int highLimit);
void generateNeccessaryCells();

enum StartMenuOptions
{
	NewGame = 1,
	ContinueGame = 2
};

struct Cell
{
	char Symbol = OperationSymbols[0];
	int Value = -1;
	char SteppedOnBy = CellNotSteppedOn;
	int ColorValue = TextWhite;
};

struct Player
{
	double Points;
	int ColorValue;
	// Cell CurrentCell;
};

void printGrid()
{
	for (int i = 0; i < sizeRows; i++)
	{
		for (int j = 0; j < sizeColumns * 4; j++) cout << "_";

		cout << "\n";

		for (int j = 0; j < sizeColumns; j++)
		{
			cout << grid[i][j].Symbol << grid[i][j].Value << grid[i][j].SteppedOnBy << " ";
		}

		cout << "\n";
	}
}

void clearConsole() {
	cout << "\033[;H";
	cout << "\033[J";
}

void clearInputBuffer() {
	cin.clear();
	cin.sync();
	cin.ignore();
}

void showStartMenu()
{
	cout << "Welcome to MathTricks!\n";
	cout << "Enter '" << NewGame << "' to start a brand new game.\n";
	cout << "Enter '" << ContinueGame << "' to read from a file and continue a game.\n";
	cout << "Enter something else to quit.\n";
}

void readFromFile()
{

}

void initializeGrid(int rows, int cols) {
	grid = new Cell * [rows];

	for (int i = 0; i < rows; ++i) {
		grid[i] = new Cell[cols];
	}
}

void deleteGrid(int rows) {
	for (int i = 0; i < rows; ++i) {
		delete[] grid[i];
	}
	delete[] grid;
}

void generateGameSeed()
{
	initializeGrid(sizeRows, sizeColumns);

	generateNeccessaryCells();

	for (int i = 0; i < sizeRows; i++)
	{
		for (int j = 0; j < sizeColumns; j++)
		{
			if (grid[i][j].Value == -1) grid[i][j] = generateCell();
		}
	}
}

void generateNeccessaryCells()
{
	Cell zeroCell = generateCell('+', 0);

	Cell multZero = generateCell('*', 0);
	Cell multTwo = generateCell('*', 2);
	Cell divTwo = generateCell('/', 2);
	Cell addOne = generateCell('+', 1);
	Cell subOne = generateCell('-', 1);

	Cell specials[] = { multZero, multTwo, divTwo, addOne, subOne };

	grid[0][0] = zeroCell;
	grid[sizeRows - 1][sizeColumns - 1] = zeroCell;

	int neccessaryCellsCount = 5;

	while (neccessaryCellsCount > 0)
	{
		int ranRow = generateTrulyRandomNumber(0, sizeRows - 1);
		int ranCol = generateTrulyRandomNumber(0, sizeColumns - 1);

		if (grid[ranRow][ranCol].Value != -1)
		{
			continue;
		}

		grid[ranRow][ranCol] = specials[--neccessaryCellsCount];
	}
}

Cell generateCell(char symbol, int value)
{
	Cell cell = { 0 };

	if (symbol != '=' || value != -1)
	{
		cell = { symbol, abs(value) };
		return cell;
	}

	int minValueRan = 0;
	int maxValueRan = max(sizeRows, sizeColumns);

	char operation = OperationSymbols[generateTrulyRandomNumber(0, OperationsCount - 1)];

	if (operation == '*')
	{
		maxValueRan = min(sizeRows, sizeColumns) / 2;
	}

	if (operation == '/')
	{
		minValueRan = 2;
		maxValueRan = min(sizeRows, sizeColumns) / 2;
	}

	int cellValue = generateTrulyRandomNumber(minValueRan, maxValueRan);

	cell = { operation, cellValue };

	return cell;
}

int generateTrulyRandomNumber(int lowLimit, int highLimit)
{
	if (lowLimit >= highLimit)
	{
		return lowLimit;
	}

	random_device rdNum;
	mt19937 genNum(rdNum());
	uniform_int_distribution<> distNum(lowLimit, highLimit);

	return distNum(genNum);
}

void getFieldSize(int& rows, int& columns)
{
	cout << "You chose to generate a new game.\n";

	cout << "Enter the number of rows for the field. ";
	do
	{
		cout << "Range: " << FieldSizeMin << " to " << FieldSizeMax << "\n";
		cin >> rows;
	} while (cin.fail() || rows < FieldSizeMin || rows > FieldSizeMax);
	cin.clear();

	cout << "Enter the number of columns for the field: \n";
	do
	{
		cout << "Range: " << FieldSizeMin << " to " << FieldSizeMax << "\n";
		cin >> columns;
	} while (cin.fail() || rows < FieldSizeMin || rows > FieldSizeMax);
	cin.clear();
}

int main()
{
	showStartMenu();

	int gameInitOption;
	cin >> gameInitOption;
	clearConsole();

	switch (gameInitOption)
	{
	case NewGame:
		getFieldSize(sizeRows, sizeColumns);
		generateGameSeed();
		break;
	case ContinueGame:
		readFromFile();
		break;
	default:
		cout << "Bye!";
		return 0;
		break;
	}

	printGrid();

	deleteGrid(sizeRows);
}
