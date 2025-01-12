#include <random>

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
