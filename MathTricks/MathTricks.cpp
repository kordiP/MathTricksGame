#include <iomanip>
#include <iostream> // console io
#include <random>
#include <windows.h> // console colors
#include <fstream>
#include "Constants.h"
#include "HelperFunctions.h"

using namespace std;

int sizeRows;
int sizeColumns;

enum StartMenuOptions;
struct Cell;
struct Player;

ofstream outStream;
ifstream inStream;

Cell** grid = nullptr;

enum StartMenuOptions
{
	NewGame = 1,
	ContinueGame = 2
};

struct Cell
{
	char Symbol = '+'; // Which operation we will apply to the player's points
	int Value = -1; // The numeric value for the operation

	int ColorValue = ColorWhite; // What color should we color the cell
	bool SteppedOn = false; // If it has been stepped on
};

struct Player
{
	int ColorAttribute = ColorWhite; // Player individual color: foreground + 16 * background
	char Name[20] = ""; // Not chosen by player, assigned by default
	double Points = 0.0; // Current player score
	int CurRow = -1; // Current row of the player
	int CurColumn = -1; // Current column of the player
};

Player playerA;
Player playerB;

void clearInputBuffer() {
	cin.clear();
	cin.sync();
	cin.ignore();
}

void printDividingLine(int dashes)
{
	for (int i = 0; i < sizeColumns; i++)
	{
		cout << "+";
		for (int h = 0; h < dashes; h++)
		{
			cout << "-";
		}
	}

	cout << "+\n";
}

void printGrid()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int spaceForCell = 3;
	int dashes = spaceForCell + 2; // +2 symbols -> whitespace and sign

	for (int i = 0; i < sizeRows; i++)
	{
		printDividingLine(dashes);
		cout << "|";

		for (int j = 0; j < sizeColumns; j++)
		{
			SetConsoleTextAttribute(hConsole, grid[i][j].ColorValue);
			cout << " " << left << grid[i][j].Symbol << setw(spaceForCell) << grid[i][j].Value;
			SetConsoleTextAttribute(hConsole, ColorWhite);

			cout << "|";
		}

		cout << "\n";
	}

	printDividingLine(dashes);
}

void showStartMenu()
{
	cout << "Welcome to MathTricks!\n";
	cout << "Enter \'" << NewGame << "\' to start a brand new game.\n";
	cout << "Enter \'" << ContinueGame << "\' to read from a file and continue a game.\n";
	cout << "Enter something else to quit.\n";
}

void deleteGrid(int rows) {
	for (int i = 0; i < rows; ++i) {
		delete[] grid[i];
	}
	delete[] grid;
}

void initializePlayers()
{
	playerA.ColorAttribute = ColorYellow + 16 * ColorBlue;
	playerA.CurRow = 0;
	playerA.CurColumn = 0;
	stringCopy(playerA.Name, "Player 1 (blue)");
	grid[playerA.CurRow][playerA.CurColumn].SteppedOn = true;
	grid[playerA.CurRow][playerA.CurColumn].ColorValue = playerA.ColorAttribute;

	playerB.ColorAttribute = ColorYellow + 16 * ColorGreen;
	playerB.CurRow = sizeRows - 1;
	playerB.CurColumn = sizeColumns - 1;
	stringCopy(playerB.Name, "Player 2 (green)");
	grid[playerB.CurRow][playerB.CurColumn].SteppedOn = true;
	grid[playerB.CurRow][playerB.CurColumn].ColorValue = playerB.ColorAttribute;
}

void readFromFile()
{
	// outStream / inStream actions
}

bool isValidPlayerMove(Player player, int rowTo, int colTo)
{
	if (rowTo >= sizeRows || rowTo < 0)
	{
		return false;
	}

	if (colTo >= sizeColumns || colTo < 0)
	{
		return false;
	}

	int curRow = player.CurRow;
	int curCol = player.CurColumn;

	int diffRow = rowTo - curRow;
	int diffCol = colTo - curCol;

	if (absNum(diffRow) > 1 || absNum(diffCol) > 1)
	{
		return false;
	}

	if (diffRow == 0 && diffCol == 0)
	{
		return false;
	}

	if (grid[rowTo][colTo].SteppedOn)
	{
		return false;
	}

	return true;
}

void initializeGrid(int rows, int cols) {
	grid = new Cell * [rows];

	for (int i = 0; i < rows; ++i) {
		grid[i] = new Cell[cols];
	}
}

Cell generateCell(char symbol = '=', int value = -1, bool hasReachedMaxZeros = false)
{
	Cell cell = { 0 };

	if (symbol != '=' || value != -1)
	{
		cell = { symbol, absNum(value) };
		return cell;
	}

	int minValueRan = 0;
	if (hasReachedMaxZeros) minValueRan = 1;

	int maxValueRan = maxNum(sizeRows, sizeColumns);

	char operation = OperationSymbols[genRandomNum(0, OperationsCount - 1)];

	if (operation == '*')
	{
		maxValueRan = minNum(sizeRows, sizeColumns) / 2;
	}

	if (operation == '/')
	{
		minValueRan = 2;
		maxValueRan = minNum(sizeRows, sizeColumns) / 2;
	}

	int cellValue = genRandomNum(minValueRan, maxValueRan);

	cell = { operation, cellValue };

	return cell;
}

void generateNeccessaryCells()
{
	Cell zeroCell = generateCell('+', 0);

	Cell multZero = generateCell('*', 0);
	Cell multTwo = generateCell('*', 2);
	Cell divTwo = generateCell('/', 2);
	Cell addOne = generateCell('+', 1);
	Cell subOne = generateCell('-', 1);

	Cell neccessaryCells[] = { multZero, multTwo, divTwo, addOne, subOne };

	grid[0][0] = zeroCell;
	grid[sizeRows - 1][sizeColumns - 1] = zeroCell;

	int neccessaryCellsCount = 5;

	while (neccessaryCellsCount > 0)
	{
		int ranRow = genRandomNum(0, sizeRows - 1);
		int ranCol = genRandomNum(0, sizeColumns - 1);

		if (grid[ranRow][ranCol].Value != -1)
		{
			continue;
		}

		grid[ranRow][ranCol] = neccessaryCells[--neccessaryCellsCount];
	}
}

void generateGameField()
{
	initializeGrid(sizeRows, sizeColumns);

	generateNeccessaryCells();

	// many grids where ~50% of cells had value of 0, so I limit them
	int maxZeros = minNum(sizeRows, sizeColumns) / 2 - 1;
	bool hasReachedMaxZeros = false;

	for (int i = 0; i < sizeRows; i++)
	{
		for (int j = 0; j < sizeColumns; j++)
		{
			if (grid[i][j].Value == -1)
			{
				grid[i][j] = generateCell('=', -1, hasReachedMaxZeros);

				if (maxZeros <= 0) hasReachedMaxZeros = true;
				else if (grid[i][j].Value == 0) maxZeros--;
			}
		}
	}

	initializePlayers();
}

bool movePlayerIfPossible(Player& player, int rowTo, int colTo)
{
	if (!isValidPlayerMove(player, rowTo, colTo))
	{
		return false;
	}
	
	int& curRow = player.CurRow;
	int& curCol = player.CurColumn;

	int diffRow = rowTo - curRow;
	int diffCol = colTo - curCol;

	grid[curRow][curCol].ColorValue -= ColorYellow;

	curRow += diffRow;
	curCol += diffCol;

	Cell& curCell = grid[curRow][curCol];

	curCell.SteppedOn = true;
	curCell.ColorValue = player.ColorAttribute;

	switch (curCell.Symbol)
	{
	case '+':
		player.Points += curCell.Value;
		break;
	case '-':
		player.Points -= curCell.Value;
		break;
	case '*':
		player.Points *= curCell.Value;
		break;
	case '/':
		player.Points /= curCell.Value;
		break;
	}

	return true;
}

bool playerHasValidMoves(Player player)
{
	int curRow = player.CurRow;
	int curCol = player.CurColumn;

	int countValid = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (isValidPlayerMove(player, curRow + i, curCol + j))
			{
				countValid++;
			}
		}
	}

	return countValid;
}

void getFieldSize()
{
	cout << "You chose to generate a new game.\n";

	cout << "Enter the number of rows for the field. ";
	do
	{
		clearInputBuffer();
		cout << "Range: " << FieldSizeMin << " - " << FieldSizeMax << "\n";
		cin >> sizeRows;
	} while (cin.fail() || sizeRows < FieldSizeMin || sizeRows > FieldSizeMax);

	cout << "Enter the number of columns for the field: \n";
	do
	{
		clearInputBuffer();
		cout << "Range: " << FieldSizeMin << " - " << FieldSizeMax << "\n";
		cin >> sizeColumns;
	} while (cin.fail() || sizeColumns < FieldSizeMin || sizeColumns > FieldSizeMax);
	
	clearConsole();
}

void printPlayerDetails(Player pl)
{
	cout << pl.Name << " @<" << pl.CurRow << ", " << pl.CurColumn << "> " << ": " << pl.Points << " points\n";
}

void startProgram()
{
	showStartMenu();

	int gameInitOption;
	cin >> gameInitOption;
	clearConsole();

	switch (gameInitOption)
	{
	case NewGame:
		getFieldSize();
		generateGameField();
		break;
	case ContinueGame:
		readFromFile();
		break;
	default:
		cout << "Exited.";
		return;
		break;
	}

	printGrid();
	Player* playerToMove = &playerA;

	do
	{
		printPlayerDetails(*playerToMove);

		int rowTo, colTo;
		cin >> rowTo;
		cin >> colTo;

		if (!movePlayerIfPossible(*playerToMove, rowTo, colTo))
		{
			cout << "Invalid move, please type a valid spot;\n";
			continue;
		}
		else
		{
			if (playerToMove == &playerA) playerToMove = &playerB;

			else playerToMove = &playerA;
			
		}

		clearConsole();
		printGrid();
	} while (playerHasValidMoves(*playerToMove));

	// Save winner in the playerToMove and print
	playerToMove = (playerA.Points > playerB.Points) ? &playerA : &playerB;
	cout << playerToMove->Name << " is the winner with " << playerToMove->Points << " points.\n";

	// Switch to loser and print
	playerToMove = (playerToMove == &playerA) ? &playerB : &playerA;
	cout << playerToMove->Name << " is second with " << playerToMove->Points << " points.\n";

	deleteGrid(sizeRows);
}

/*
Останали:
- deserialize във файл и след това serialize - запазване на игра
- fair game logic -> closer to center -> better chance for a big num (+/*)
- валидиране на всички възможни входни полета
- добавяне на коментари за изясняване на кода
*/