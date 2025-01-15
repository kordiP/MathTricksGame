/*
*
* Solution to course project # 2
* Introducution To Programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter Semester 2024/2025
* 
* @author Atanas Kordov
* @idnumber 5MI0600508
* @compiler VC
* 
* file for main game logic
* 
*/

#include <iomanip> // setw io
#include <random> // truly random
#include <windows.h> // console colors
#include <cmath> // for sqrt
#include <fstream>
#include <iostream>
#include "Constants.h"
#include "HelperFunctions.h"

using namespace std;

int sizeRows;
int sizeColumns;

enum StartMenuOptions
{
	NewGame = 1,
	ContinueGame = 2,
	SaveGame = -1
};

struct Cell
{
	char Operation = '+'; // Which operation we will apply to the player's points
	int Value = -1; // The numeric value for the operation

	int ColorValue = COLOR_WHITE; // What color should we color the cell
	bool SteppedOn = false; // If it has been stepped on
};

struct Player
{
	int ColorAttribute = COLOR_WHITE; // Player individual color: foreground + 16 * background
	char Name[20] = "Player (undef)"; // Not chosen by player, assigned by default
	double Points = 0.0; // Current player score
	int CurRow = -1; // Current row of the player
	int CurColumn = -1; // Current column of the player
};

Cell** grid = nullptr;

Player playerA;
Player playerB;

void clearInputBuffer() {
	cin.clear();
	cin.sync();
	cin.ignore();
}

// Prints the table's horizontal dividers.
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
	// for changes in color of console
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
			// sets color based on an integer. Values are predefined for the library
			SetConsoleTextAttribute(hConsole, grid[i][j].ColorValue);
			cout << " " << left << grid[i][j].Operation << setw(spaceForCell) << grid[i][j].Value;
			SetConsoleTextAttribute(hConsole, COLOR_WHITE);

			cout << "|";
		}

		cout << "\n";
	}

	printDividingLine(dashes);
}

void printStartMenu()
{
	cout << "Welcome to MathTricks!\n";
	cout << "Enter \'" << NewGame << "\' to start a brand new game.\n";
	cout << "Enter \'" << ContinueGame << "\' to continue last game.\n";
	cout << "Enter something else to quit.\n";
	cout << "*Enter \'" << SaveGame << "\' at any point in the game to save game. Previously saved game will be lost.\n";
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

void setPlayer(Player& pl, int colorAtt, int curRow, int curCol, const char* name, double points = 0.0)
{
	pl.ColorAttribute = colorAtt; 
	pl.CurRow = curRow;
	pl.CurRow = curCol;
	pl.Points = points;
	stringCopy(pl.Name, name);
	grid[curRow][curCol].SteppedOn = true;
	grid[curRow][curCol].ColorValue = pl.ColorAttribute;
}

// Setup both players with predefined values.
void setupPlayersDefault()
{
	setPlayer(playerA, PLAYER_A_COLOR, 0, 0, PLAYER_A_NAME);
	setPlayer(playerB, PLAYER_B_COLOR, sizeRows - 1, sizeColumns - 1, PLAYER_B_NAME);
}

// Read last played game.
void readGridFromFile(Player* plToMove) {
	int plACurRow = 0;
	int plACurCol = 0;
	double plAPoints = 0.0;

	int plBCurRow = 0;
	int plBCurCol = 0;
	double plBPoints = 0.0;

	bool isPlAMove = false;

	std::ofstream saveFile("lastGame.txt");
	if (saveFile.is_open()) {
		saveFile << "P1: " << plACurRow << ',' << plACurCol << ',' << plAPoints << '\n';

		saveFile << "P2: " << plBCurRow << ',' << plBCurCol << ',' << plBPoints << '\n';

		saveFile << "A-MOVES: " << isPlAMove << '\n';

		saveFile << "GRID: \n" << sizeRows << '\n' << sizeColumns << '\n';

		initializeGrid(sizeRows, sizeColumns);
		// +,0,158,1
		saveFile << "\n";

		// read the grid and done

		setPlayer(playerA, PLAYER_A_COLOR, plACurRow, plACurCol, PLAYER_A_NAME, plAPoints);
		setPlayer(playerB, PLAYER_B_COLOR, plBCurRow, plBCurCol, PLAYER_B_NAME, plBPoints);

		saveFile.close();
		return;
	}
}

void saveGridToFile(bool playerAToMove)
{
	ofstream saveFile;
	saveFile.open("saveFile.txt");

	saveFile << "P1: " << playerA.CurRow << "," << playerA.CurColumn << "," << playerA.Points << "\n";
	saveFile << "P2: " << playerB.CurRow << "," << playerB.CurColumn << "," << playerB.Points << "\n";
	saveFile << "A-MOVES: " << playerAToMove << "\n";
	saveFile << "GRID: " << sizeRows << "," << sizeColumns << "\n";

	for (int i = 0; i < sizeRows; i++)
	{
		for (int j = 0; j < sizeColumns; j++)
		{
			Cell curCell = grid[i][j];
			saveFile << curCell.Operation << "," << curCell.Value << "," << curCell.ColorValue << "," << curCell.SteppedOn << " ";
		}
		saveFile << "\n";
	}

	saveFile.close();
}

// Checks if player's input is a valid move.
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

// Calculate current cell position relative to the middle.
double differenceFromMiddle(int curRow, int curCol)
{
	// if it's one of the neccessary cells
	if (curRow < 0 || curCol < 0)
	{
		return 0;
	}

	int centerRow = sizeRows / 2;
	int centerCol = sizeColumns / 2;

	// calc distance with Euclidean method
	return sqrt((curRow - centerRow) * (curRow - centerRow) + (curCol - centerCol) * (curCol - centerCol));
}

// Generate cells randomly with option to give it specific values.
Cell generateCell(char symbol = '=', int value = -1, int curRow = -1, int curCol = -1)
{
	Cell cell = { 0 };

	if (symbol != '=' || value != -1)
	{
		cell = { symbol, value };
		return cell;
	}

	int minValueRan = 0;
	int maxValueRan = maxNum(sizeRows, sizeColumns);

	char operation = OPERATIONS_ARR[genRandomNum(0, OPERATIONS_COUNT - 1)];

	double dist = differenceFromMiddle(curRow, curCol); 
	// can be optimized, no need to run for every cell (1)
	double maxDist = differenceFromMiddle(0, 0); 

	// value is in interval: [0, 1]. Closer to 0 == Closer to center. 1 is the most distant point
	double normDist = dist / maxDist;

	if (operation == '+' || operation == '-')
	{
		int penMax = maxValueRan - CELL_VAL_RANGE; // the absolute maximum penalty for a cell + same as (1)
		double penalty = normDist * penMax;

		maxValueRan -= ceilNum(penalty);
		minValueRan = maxValueRan - CELL_VAL_RANGE;
	}
	else if (operation == '/' || operation == '*')
	{
		minValueRan = 2;
		maxValueRan = MULTIPLIER_MAX;

		if (normDist >= 0.5) maxValueRan--;
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

	for (int i = 0; i < sizeRows; i++)
	{
		for (int j = 0; j < sizeColumns; j++)
		{
			if (grid[i][j].Value == -1)
			{
				grid[i][j] = generateCell('=', -1, i, j);
			}
		}
	}
}

// Checks if player's input is a possible move and do move.
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

	grid[curRow][curCol].ColorValue -= COLOR_YELLOW;

	curRow += diffRow;
	curCol += diffCol;

	Cell& curCell = grid[curRow][curCol];

	curCell.SteppedOn = true;
	curCell.ColorValue = player.ColorAttribute;

	switch (curCell.Operation)
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

// Gets user input for field size.
void getFieldSize()
{
	cout << "You chose to generate a new game.\n";

	cout << "Enter the number of rows for the field. ";
	do
	{
		clearInputBuffer();
		cout << "Range: " << GRID_SIZE_MIN << " - " << GRID_SIZE_MAX << "\n";
		cin >> sizeRows;
	} while (cin.fail() || sizeRows < GRID_SIZE_MIN || sizeRows > GRID_SIZE_MAX);

	cout << "Enter the number of columns for the field: \n";
	do
	{
		clearInputBuffer();
		cout << "Range: " << GRID_SIZE_MIN << " - " << GRID_SIZE_MAX << "\n";
		cin >> sizeColumns;
	} while (cin.fail() || sizeColumns < GRID_SIZE_MIN || sizeColumns > GRID_SIZE_MAX);
	
	clearConsole();
}

void printPlayerDetails(Player pl)
{
	cout << pl.Name << " @<" << pl.CurRow << ", " << pl.CurColumn << "> " << ": " << pl.Points << " points\n";
}

void printWinner()
{
	if (playerA.Points == playerB.Points)
	{
		cout << "Tie game! Both players had " << playerA.Points << " points!";
	}
	else if (playerA.Points > playerB.Points)
	{
		cout << playerA.Name << " is the winner with " << playerA.Points << "!\n";
		cout << playerB.Name << " had " << playerB.Points << ".\n";
	}
	else
	{
		cout << playerB.Name << " is the winner with " << playerB.Points << "!";
		cout << playerB.Name << " had " << playerB.Points << ".\n";
	}
}

// Clear buffers and trigger file save
void endGameWithSave(Player* plToMove)
{
	clearConsole();
	clearInputBuffer();

	saveGridToFile(plToMove == &playerA);
	cout << "Game saved in local file.";

	deleteGrid(sizeRows);
}

void startProgram()
{
	printStartMenu();

	int gameInitOption;
	cin >> gameInitOption;
	clearConsole();

	Player* playerToMove = &playerA;

	switch (gameInitOption)
	{
	case NewGame:
		getFieldSize();
		generateGameField();
		break;
	case ContinueGame:
		readGridFromFile(playerToMove);
		break;
	default:
		cout << "Exited.";
		return;
		break;
	}
	printGrid();

	do
	{
		printPlayerDetails(*playerToMove);
		int rowTo, colTo;

		cin >> rowTo;
		if (rowTo == -1)
		{
			endGameWithSave(playerToMove);
			return;
		}

		cin >> colTo;
		if (colTo == -1)
		{
			endGameWithSave(playerToMove);
			return;
		}

		if (!movePlayerIfPossible(*playerToMove, rowTo, colTo))
		{
			cout << "Invalid move, please type a valid spot.\n";
			clearInputBuffer();
			continue;
		}
		else
		{
			if (playerToMove == &playerA) playerToMove = &playerB;

			else playerToMove = &playerA;
		}

		clearInputBuffer();
		clearConsole();
		printGrid();
	} while (playerHasValidMoves(*playerToMove));

	printWinner();
	deleteGrid(sizeRows);
}

/*
Останали:
- валидиране на всички възможни входни полета
- change commits names
- deserialize only grid left
- check for magic numbers
*/