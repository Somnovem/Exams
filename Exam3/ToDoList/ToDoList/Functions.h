#pragma once
#include<iostream>
#include <windows.h>
using namespace std;

template<class T>
int findKey(T* arr, int size, T key)
{
	for (int i = 0; i < size; i++)
	{
		if (arr[i] == key)
			return i;
	}
	return -1;
}


template<class T>
void addElemArray(T*& arr, int& size, T elem)
{
	T* temp = new T[size + 1];
	for (size_t i = 0; i < size; i++)
	{
		temp[i] = arr[i];
	}
	temp[size] = elem;
	size++;
	delete[] arr;
	arr = temp;
}

template<class T>
void delElemArray(T*& arr, int& size, int pos)
{
	T* temp = new T[size - 1];
	for (size_t i = 0; i < pos; i++)
	{
		temp[i] = arr[i];
	}
	for (size_t i = pos + 1; i < size; i++)
	{
		temp[i - 1] = arr[i];
	}
	size--;
	delete[] arr;
	arr = temp;
}

template<class T>
void printArray(const T* arr, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

enum HorizontalAlignment
{
	Center, Left, Right
};

enum ConsoleColor
{
	Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Brown = 6, LightGray = 7, DarkGray = 8,
	LightBlue = 9, LightGreen = 10, LightCyan = 11, LightRed = 12, LightMagenta = 13, Yellow = 14, White = 15
};

void SetColor(int text, int background);

void gotoxy(int x, int y);

template<class T>
bool asc(T a, T b)
{
	return a > b;
}

template<class T>
bool desc(T a, T b)
{
	return a < b;
}

template<class T>
bool evenFirst(T a, T b)
{
	if (a % 2 == 1 && b % 2 == 0)
		return false;
	if (a % 2 == 0 && b % 2 == 1)
		return true;
	return asc(a, b);
}