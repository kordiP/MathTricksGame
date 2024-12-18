#include <fstream> // file io
#include <iostream> // console io
#include <windows.h> // console colors
#include <ctime> // rng seed

using namespace std;

#pragma region GlobalVars

const int FieldSizeMin = 4;
const int FieldSizeMax = 14;

const int OperationsCount = 4;
const char OperationSymbols[OperationsCount] = {'+', '-', '*', '/'};

const char SeedCellSeperator = '_';
const char SeedCellNotSteppedOn = 'N';
const char SeedCellSteppedOnByA = 'A';
const char SeedCellSteppedOnByB = 'B';

int gameInitOption;
int sizeRows;
int sizeColumns;

enum StartMenuOptions
{
    NewGame = 1,
    ContinueGame
};

#pragma endregion

#pragma region Functions

void showStartMenu()
{
    cout << "Welcome to MathTricks!\n";
    cout << "Enter '1' to start a brand new game.\n";
    cout << "Enter '2' to read from a file and continue a game.\n";
    cout << "Enter something else to quit.\n";
}

char intToChar(int input)
{
    if (input > 9)
    {
        return '9';
    }

    return input + '0';
}

void generateGameSeed(int rows, int columns)
{    
    int index = 0;
    srand(time(0));

    int maxValue = min(9, max(rows, columns));

    char operation = OperationSymbols[rand() % OperationsCount];
    int value = rand() % maxValue;

    char valueChar = intToChar(value);
}

void getFieldSize(int& rows, int& columns)
{
    cout << "You chose to generate a new game.\n";

    cout << "Enter the number of rows for the field. ";
    do
    {
        cout << "Range: " << FieldSizeMin << " to " << FieldSizeMax <<  "\n";
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

void readFromFile()
{

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

#pragma endregion



int main()
{
    showStartMenu();
    cin >> gameInitOption;
    clearConsole();

    switch (gameInitOption)
    {
    case NewGame:
        getFieldSize(sizeRows, sizeColumns);
        generateGameSeed(sizeRows, sizeColumns);
        break;
    case ContinueGame:
        readFromFile();
        break;
    default:
        cout << "Bye!";
        return 0;
        break;
    }

}
