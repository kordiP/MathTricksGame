#include <cstdlib>
#include <ctime> // rng seed
#include <iostream> // console io
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
char intToChar(int input);
void generateGameSeed();
Cell generateCell(int curRow, int curCol);
void getFieldSize(int& rows, int& columns);
void clearConsole();
void clearInputBuffer();

enum StartMenuOptions
{
	NewGame = 1,
	ContinueGame = 2
};

struct Cell
{
	char Symbol = OperationSymbols[0];
	int Value = 0;
	char SteppedOnBy = CellNotSteppedOn;
	int ColorValue = TextWhite;
};

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

char intToChar(int input)
{
	if (input > 9)
	{
		return '9';
	}

	return input + '0';
}

void generateGameSeed()
{
	initializeGrid(sizeRows, sizeColumns);

	for (int i = 0; i < sizeRows; i++)
	{
		for (int j = 0; j < sizeColumns; j++)
		{
			grid[i][j] = generateCell(i, j);
		}
	}
}

Cell generateCell(int curRow, int curCol, char symbol = '+', int value = -1)
{
	srand(time(0));
	Cell cell;

	if (curRow == 0 && curCol == 0)
	{
		return cell;
	}

	else if (curRow == sizeRows - 1 && curCol == sizeColumns - 1)
	{
		return cell;
	}

	int maxValue = max(sizeRows, sizeColumns);

	char operation = OperationSymbols[rand() % OperationsCount];
	int value = rand() % maxValue;

	cell = { operation, value };

	return cell;
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
		cout << "Range: 4 to 12. \n";
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

	deleteGrid(sizeRows);
}
