#include <iostream>
#include <ctime>
#include <time.h>
#include <random>
#include <iomanip>
#include <fstream>
int sizeN = 0;
long long moves = 0;
/// <summary>
/// Checks if the number can be put into the grid
/// </summary>
/// <param name="arr"> - the grid</param>
/// <param name="fill"> - the number</param>
/// <returns>1 or 0</returns>
bool checkIfCanInput(int arr[], int fill) {
	if (fill < 0 || fill >= sizeN)
	{
		return  0;
	}
	for (int i = 0; i < sizeN; i++)
	{
		if (arr[i] == fill)
		{
			return 0;
		}
	}
	return 1;
}
/// <summary>
/// Displays the grid
/// </summary>
/// <param name="arr"> - the grid</param>
void print(int arr[]) {
	int newLine = 0;
	if (sizeN > 9)
	{
		newLine = 4;
	}
	else
	{
		newLine = 3;
	}
	for (int i = 1; i <= sizeN; i++)
	{
		std::cout << std::left << std::setw(5) << arr[i - 1] << "  ";
		if (i % newLine == 0)
		{
			std::cout << std::endl;
		}
	}
}
/// <summary>
/// Proccess of filling the grid manually
/// </summary>
/// <param name="field"> - the grid</param>
void manualFill(int*& field) {
	for (int i = 0; i < sizeN-1;)
	{
		print(field);
		int place = 0;
		int filling = 0;
		std::cout << "Choose where to place a number : ";
		std::cin >> place;
		std::cout << "Choose a number from 0 to " << sizeN-1 << " : ";
		std::cin >> filling;
		if (checkIfCanInput(field, filling) && place >= 1 && place <= sizeN)
		{
			field[place - 1] = filling;
			i++;
		}
		else
		{
			std::cout << "This number is already in the grid, is out of range or the place is not in the array" << std::endl;
		}
		std::system("pause");
		std::system("cls");
	}
}
/// <summary>
/// Fills the grid with appropriate random numbers without repeating
/// </summary>
/// <param name="field"> - the grid</param>
void randomFill(int*& field) {

	for (int i = 0; i < sizeN-1;)
	{
		int filling = rand() % (sizeN-1) + 1;
		if (checkIfCanInput(field, filling))
		{
			field[i] = filling;
			i++;
		}
	}
	print(field);
	std::system("pause");
	std::system("cls");
}
/// <summary>
/// Checks wether the game is completed
/// </summary>
/// <param name="arr"> - the grid</param>
/// <returns> 1 or 0</returns>
bool check(int arr[]) {
	for (int i = 0; i < sizeN-2; i++)
	{
		if (arr[i+1] - arr[i] >=2 || arr[i + 1] - arr[i] < 0)
		{
			return 0;
		}
	}
	if (arr[sizeN-1] != 0)
	{
		return 0;
	}
	return 1;
}
/// <summary>
/// Looks for the number in the grid
/// </summary>
/// <param name="arr"> - the grid</param>
/// <param name="value"> - the number</param>
/// <returns> pos of the num in the grid</returns>
int findAValue(int arr[], int value) {
	for (int i = 0; i < sizeN; i++)
	{
		if (arr[i] == value)
		{
			return i;
		}
	}
	return NULL;
}
/// <summary>
/// Finds the zero in the grid
/// </summary>
/// <param name="arr"> - the grid</param>
/// <returns>pos of zero in the grid</returns>
int findZero(int arr[]) {
	for (int i = 0; i < sizeN; i++)
	{
		if (arr[i] == 0)
		{
			return i;
		}
	}
}
/// <summary>
/// Proccess of player playing
/// </summary>
/// <param name="field"> - the field</param>
void playerIsPlaying(int*& field) {
	int lineMoving = (sizeN > 9) ? 4 : 3;
	int value = 0;
	int found = 0;
	if (sizeN > 9)
	{
		while (!check(field))
		{
			int zero = findZero(field);
			print(field);
			std::cout << "What value do you wanna move: ";
			std::cin >> value;
			found = findAValue(field, value);
			if (found == zero + 1 && zero + 1 != 4 && zero + 1 != 8 && zero + 1 != 12 || found == zero - 1 && zero - 1 != 3 && zero - 1 != 7 && zero-1 !=11 || found == zero + lineMoving || found == zero - lineMoving)
			{
				std::swap(field[found], field[zero]);
				moves++;
			}
			else
			{
				std::cout << "Can only swap with zero" << std::endl;
				system("pause");
			}
			system("cls");
		}
	}
	else
	{
		while (!check(field))
		{
			int zero = findZero(field);
			print(field);
			std::cout << "What value do you wanna move: ";
			std::cin >> value;
			found = findAValue(field, value);
			if (found == zero + 1 && zero+1 != 3 && zero +1 != 6 || found == zero - 1 && zero - 1 != 2 && zero -1 != 5 || found == zero + lineMoving || found == zero - lineMoving)
			{
				std::swap(field[found], field[zero]);
				moves++;
			}
			else
			{
				std::cout << "Can only swap with zero" << std::endl;
				system("pause");
			}
			system("cls");
		}
	}
	
}
int main() {
	int record = 0;
	std::string filename = "out.txt";
	std::ifstream fileIn;
	fileIn.open(filename);
	if (fileIn.fail())
	{
		std::cout << "Could not open the file" << std::endl;
		return 404;
	}
	fileIn >> record;
	fileIn.close();
	srand(time(NULL));
	int choice = 0;
	time_t time_start;
	time_t time_end;
l1:
	moves = 0;
	system("cls");
	std::cout << "Choose the game mode:\n 1 - 3x3\n 2 - 4x4\n";
	std::cin >> choice;
	switch (choice)
	{
	case 1:
		sizeN = 9;
		break;
	case 2:
		sizeN = 16;
		break;
	default:
		std::cout << "INVALID INPUT" << std::endl;
		goto l1;
		break;
	}
	int* field = new int[sizeN] {};
l2: 
	system("cls");
	std::cout << "Choose how to fill in the grid:\n 1 - manually\n 2 - randomly\n";
	std::cin >> choice;
	switch (choice)
	{
	case 1:
		manualFill(field);
		break;
	case 2:
		randomFill(field);
		break;
	default:
		std::cout << "INVALID INPUT" << std::endl;
		goto l2;
		break;
	}
		time(&time_start);
		playerIsPlaying(field);
		time(&time_end);
		print(field);
		std::cout << "Congrats!" << std::endl;
		std::cout << "Game lasted for " <<time_end - time_start <<" seconds." << std::endl;
		std::cout << "It took " << moves << " to solve the puzzle" << std::endl;
		if (moves < record)
		{
			std::cout << "You beat the record! The last record was: " << record << " moves " << std::endl;
			record = moves;
			std::ofstream fileOut;
			fileOut.open(filename);
			if (fileIn.fail())
			{
				std::cout << "Could not open the file" << std::endl;
				return 404;
			}
			fileOut << record;
			fileOut.close();
		}
		else
		{
			std::cout << "The record of " << record << " remains unbeatable. Good luck next try!" << std::endl;
		}
		std::cout << "Do you want to restart the game?\n 1 - yes\n 2 - no\n";
		std::cin >> choice;
		if (choice == 1) goto l1;
		return 0;
}

