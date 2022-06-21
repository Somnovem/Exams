#include <iostream>
#include <ctime>
#include <time.h> // Ўанс срабатывани€ 60% в 3 на 3, в 4 на 4 ты просто не дождешс€ когда он его соберет
#include <random>
#include <iomanip>
int sizeN = 0;
long long moves = 0;
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
void manualFill(int*& field) {
	for (int i = 0; i < sizeN;)
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
void randomFill(int*& field) {

	for (int i = 0; i < sizeN;)
	{
		int filling = rand() % sizeN;
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
int findZero(int arr[]) {
	for (int i = 0; i < sizeN; i++)
	{
		if (arr[i] == 0)
		{
			return i;
		}
	}
}
void pcIsPlaying(int*& field) {
	int lineMoving = (sizeN > 9) ? 4 : 3;
	int previous = 0;
	if (sizeN > 9)
	{
		while (!check(field))
		{
			int zero = findZero(field);
			int options[4]{};
			int current = 0;
			if (field[zero - 1] > 0 && zero - 1 != 3 && zero - 1 != 7 && zero - 1 != 11 && field[zero - 1] < sizeN && previous != 2)
			{
				options[current] = 1;
				current++;
			}
			if (field[zero + 1] > 0 && zero + 1 != 4 && zero + 1 != 8 && zero + 1 != 12 && field[zero + 1] < sizeN && previous != 1)
			{
				options[current] = 2;
				current++;
			}
			if (field[zero + lineMoving] > 0 && field[zero + lineMoving] < sizeN && previous != 4)
			{
				options[current] = 3;
				current++;
			}
			if (field[zero - lineMoving] > 0 && field[zero - lineMoving] < sizeN && previous != 3)
			{
				options[current] = 4;
				current++;
			}
			int moving = options[rand() % current];
			previous = moving;
			switch (moving)
			{
			case 1:
				std::swap(field[zero], field[zero - 1]);
				break;
			case 2:
				std::swap(field[zero], field[zero + 1]);
				break;
			case 3:
				std::swap(field[zero], field[zero + lineMoving]);
				break;
			case 4:
				std::swap(field[zero], field[zero - lineMoving]);
				break;
			default:
				break;
			}
			moves++;
		}
	}
	else
	{
		while (!check(field))
		{
			int zero = findZero(field);
			int options[4]{};
			int current = 0;
			if (field[zero - 1] > 0 && zero - 1 != 2 && zero - 1 != 5 && field[zero - 1] < 9 && previous != 2)
			{
				options[current] = 1;
				current++;
			}
			if (field[zero + 1] > 0 && zero + 1 != 3 && zero + 1 != 6 && field[zero + 1] < 9 && previous != 1)
			{
				options[current] = 2;
				current++;
			}
			if (field[zero + lineMoving] > 0 && field[zero + lineMoving] < 9 && previous != 4)
			{
				options[current] = 3;
				current++;
			}
			if (field[zero - lineMoving] > 0 && field[zero - lineMoving] < 9 && previous != 3)
			{
				options[current] = 4;
				current++;
			}
			int moving = options[rand() % current];
			previous = moving;
			switch (moving)
			{
			case 1:
				std::swap(field[zero], field[zero - 1]);
				break;
			case 2:
				std::swap(field[zero], field[zero + 1]);
				break;
			case 3:
				std::swap(field[zero], field[zero + lineMoving]);
				break;
			case 4:
				std::swap(field[zero], field[zero - lineMoving]);
				break;
			default:
				break;
			}
			moves++;
		}
	}

}
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
void main() {
	srand(time(NULL));
	int choice = 0;
	time_t time_start;
	time_t time_end;
l1: //≈сли режим был выбран неправильно или рестарт
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
	int* field = new int[sizeN];
l2: //≈сли способ заполнени€ был выбран неправильно
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
l3: //≈сли игрок был выбран неправильно
	system("cls");
	std::cout << "Choose who is going to play:\n 1 - player\n 2 - PC\n";
	std::cin >> choice;
	switch (choice)
	{
	case 1:
		system("pause");
		system("cls");
		time(&time_start);
		playerIsPlaying(field);
		time(&time_end);
		print(field);
		std::cout << "Congrats!" << std::endl;
		std::cout << "Game lasted for " <<time_end - time_start <<" seconds." << std::endl;
		std::cout << "It took " << moves << " to solve the puzzle" << std::endl;
		std::cout << "Do you want to restart the game?\n 1 - yes\n 2 - no\n";
		std::cin >> choice;
		if (choice == 1) goto l1;
		break;
	case 2:
		system("pause");
		system("cls");
		time(&time_start);
		pcIsPlaying(field);
		time(&time_end);
		print(field);
		std::cout << "\a\a\a\a\aGame lasted for " << time_end - time_start << " seconds." << std::endl;
		std::cout << "It took " << moves << " to solve the puzzle" << std::endl;
		std::cout << "Do you want to restart the game?\n 1 - yes\n 2 - no\n";
		std::cin >> choice;
		if (choice == 1) goto l1;
		break;
	default:
		std::cout << "INVALID INPUT" << std::endl;
		goto l3;
		break;
	}
}

