/**
*
* Solution to course project # 2
* Introducution to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter Semester 2024/2025
*
* @author Atanas Kordov
* @idnumber 5MI0600508
* @compiler VC
*
* <file for helper functions>
*
*/

// Link to Repo -> https://github.com/kordiP/MathTricksGame

#include <random>
#include <iostream>
using namespace std;

int genRandomNum(int lowLimit, int highLimit)
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

int maxNum(int num1, int num2)
{
	if (num1 > num2)
	{
		return num1;
	}

	return num2;
}

int minNum(int num1, int num2)
{
	if (num1 < num2)
	{
		return num1;
	}

	return num2;
}

int absNum(int num) 
{
	if (num < 0)
	{
		return -num;
	}

	return num;
}

int ceilNum(double num) {
	int intPart = (int)(num);
	return (num - intPart < 0.1) ? intPart : intPart + 1;
}

int stringLength(char* str)
{
	if (str == nullptr)
	{
		return -1;
	}

	int count = 0;

	while (str[count++] != '\0');

	return count;
}

void stringCopy(char* destination, const char* source)
{
	if (destination == nullptr || source == nullptr)
	{
		return;
	}

	int index = 0;

	while (source[index] != '\0')
	{
		destination[index] = source[index];
		index++;
	}

	destination[++index] = '\0';
}

void clearConsole()
{
	cout << "\033[;H"; // move cursor to top left corner
	cout << "\033[J"; // clear
}

// == strtok
char* stringSplit(char* str, char delimiter) 
{
	static char* currentPos = nullptr;

	if (str != nullptr) 
	{
		currentPos = str; 
	}

	if (currentPos == nullptr || *currentPos == '\0') 
	{
		return nullptr;
	}

	char* substr = currentPos;

	// Find the next delimiter or end of the string
	while (*currentPos != '\0' && *currentPos != delimiter) {
		currentPos++;
	}

	if (*currentPos != '\0') {
		*currentPos = '\0';  // Replace delimiter with terminator zero
		currentPos++;  // Move past the delimiter
	}

	return substr;
}

// == strchr
char* charInString(const char* str, char ch) {
	while (*str != '\0') 
	{
		if (*str == ch) 
		{
			return (char*)str;  // return prt to the char
		}
		str++;
	}
	return nullptr;  // nullptr if character is not found
}

// == atoi
int asciiToInt(const char* str) {
	if (str == nullptr)
	{
		return -1;
	}

	int result = 0;

	while (*str >= '0' && *str <= '9') {
		result = result * 10 + (*str - '0');
		str++;
	}

	return result;
}