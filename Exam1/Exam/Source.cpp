#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::string word, key, cyphered, decyphered;
	std::string temp;
	int choice = 0;
	while (true)
	{
		std::cout << "�������� ������ �� �� ������� ������� ��� ������:\n 1 - �������\n 2 - ������\n" << std::endl;
		std::cin >> choice;
		if (choice == 1)
		{
			std::string filename = "input.txt";
			std::ifstream fileIn;
			fileIn.open(filename);
			if (fileIn.fail())
			{
				std::cout << "������ �������� �����" << std::endl;
				return 404;
			}
			fileIn >> word;
			fileIn.close();
			break;
		}
		else if (choice == 2)
		{
			std::cout << "������� �����(���������� �����): ";
			std::cin >> word;
			std::cout << std::endl;
			break;
		}
		else
		{
			std::cout << "INVALID INPUT" << std::endl;
			std::system("pause");
			std::system("cls");
		}
	}
		std::cout << "�������� ��������:\n 1 - �����������\n 2 - ������������\n";
		std::cin >> choice;
		if (choice == 1)
		{
			std::cout << "������� ������� �����(���������� �����)" << std::endl;
			std::cin >> temp;
			key = word;
			if (temp > word)
			{
				int c = 0;
				for (int i = 0; i < key.size(); i++, c++)
				{
					key[i] = temp[c];
					if (c >= temp.size() - 1) c = -1;
				}
			}
			else if (temp < word) {
				for (int i = 0; i < key.size(); i++)
				{
					key[i] = temp[i];
				}
			}
			else
			{
				key = temp;
			}
			cyphered = word;
			std::cout << std::endl;
			for (int i = 0; i < key.size(); i++) {
				int lowerBar = (isupper(word[i])) ? 65 : 97;
				int upperBar = (isupper(key[i])) ? 90 : 122;
				int k = word[i % word.size()] - lowerBar;
				int m = 0;
				if ((isupper(word[i]) && isupper(key[i])) || (!isupper(word[i]) && !isupper(key[i])))
				{
					m = key[i];
				}
				else
				{
					m = key[i] + 32;
				}
				if (m + k > upperBar) {
					cyphered[i] = m + k - 26;
				}
				else {
					cyphered[i] = m + k;
				}
			}
			std::cout << cyphered << std::endl;
			char answer;
			std::cout << "������ ��������� ��������� � ����?[y/n] ";
			std::cin >> answer;
			if (answer == 'y')
			{
				std::string filename = "input.txt";
				std::ofstream fileOut;
				fileOut.open(filename);
				if (fileOut.fail())
				{
					std::cout << "������ �������� �����" << std::endl;
					return 404;
				}
				fileOut << cyphered;
				fileOut.close();
			}
		}
		else if (choice == 2)
		{
			std::cout << "������� ������� �����(���������� �����)" << std::endl;
			std::cin >> temp;
			key = word;
			if (temp < word)
			{
				int c = 0;
				for (int i = 0; i < key.size(); i++, c++)
				{
					key[i] = temp[c];
					if (c >= temp.size() - 1) c = -1;
				}
			}
			else if (temp > word) {
				for (int i = 0; i < key.size(); i++)
				{
					key[i] = temp[i];
				}
			}
			else
			{
				key = temp;
			}
			decyphered = word;
			std::cout << std::endl;
			for (int i = 0; i < decyphered.size(); ++i) {
				char c = decyphered[i];
				if (!isalpha(c)) continue;
				if ((c >= 'A' && c <= 'Z'))
				{
					decyphered[i] = (c - toupper(key[i]) + 2 * 'A') % 26 + 'A';
				}
				else if (c >= 'a' && c <= 'z') {
					decyphered[i] = (c - tolower(key[i]) + 2 * 'a') % 26 + 'a';
				}
			}
			std::cout << decyphered << std::endl;
			char answer = 0;
			std::cout << "������ ��������� ��������� � ����?[y/n] ";
			std::cin >> answer;
			if (answer == 'y')
			{
				std::string filename = "input.txt";
				std::ofstream fileOut;
				fileOut.open(filename);
				if (fileOut.fail())
				{
					std::cout << "������ �������� �����" << std::endl;
					return 404;
				}
				fileOut << decyphered;
				fileOut.close();
			}
		}
		else
		{
			std::cout << "������ �����" << std::endl;
		}
	return 0;
}