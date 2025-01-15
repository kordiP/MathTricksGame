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
* file for helper function
*
*/

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

void stringCopy(char* destination, const char* source)
{
	while (destination == nullptr || source == nullptr)
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
