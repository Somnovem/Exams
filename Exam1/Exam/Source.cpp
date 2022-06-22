#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
std::string lib;
/// <summary>
/// Generates a lib of letters
/// </summary>
/// <returns>the lib</returns>
std::string MakeLib() {
	for (int i = 65; i <= 90; i++)
	{
		lib += (char)i;
	}
	for (int i = 98; i <= 122; i++)
	{
		lib += (char)i;
	}
	for (int i = 192; i <= 255; i++)
	{
		lib += (char)i;
	}
	return lib;
}
/// <summary>
/// Finds the letter in the lib
/// </summary>
/// <param name="letter"> - the letter</param>
/// <returns>pos of the letter</returns>
int findLetterInLib(char letter) {
	for (int i = 0; i < lib.length(); i++)
	{
		if (lib[i] == letter)
		{
			return i;
		}
	}
}
/// <summary>
/// Encrypts line with the provided key
/// </summary>
/// <param name="str"> - line</param>
/// <param name="key"> - key</param>
/// <returns> Encrypted line</returns>
std::string encrypt(std::string str, std::string key) {
	int temp;
	std::string encrypted;
	for (int i = 0; i < str.length(); i++)
	{
		temp = (findLetterInLib(str[i]) + findLetterInLib(key[i])) % lib.length();
		encrypted += lib[temp];
	}
	std::cout << encrypted << std::endl;
	return encrypted;
}
/// <summary>
/// Decrypts line with the provided key
/// </summary>
/// <param name="str"> - line</param>
/// <param name="key"> - key</param>
/// <returns>Decrypted line</returns>
std::string decrypt(std::string str, std::string key) {
	int temp;
	std::string encrypted;
	for (int i = 0; i < str.length(); i++)
	{
		temp = findLetterInLib(str[i]) - findLetterInLib(key[i]);
		if (temp < 0)
		{
			temp += lib.length();
		}
		encrypted += lib[temp];
	}
	std::cout << encrypted << std::endl;
	return encrypted;
}
int main() {
	MakeLib();
	std::string word, key;
	std::string temp;
	int choice = 0;
	std::string filename = "input.txt";
	while (true)
	{
		std::cout << "Do you want to read the line or type it in:\n 1 - Read\n 2 - Type In" << std::endl;
		std::cin >> choice;
		if (choice == 1)
		{
			std::ifstream fileIn;
			fileIn.open(filename);
			if (fileIn.fail())
			{
				std::cout << "Reading failed" << std::endl;
				return 404;
			}
			fileIn >> word;
			std::cout << "The line: " << word << std::endl;
			fileIn.close();
			break;
		}
		else if (choice == 2)
		{
			std::cout << "Type in your word: ";
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
	std::cout << "Type in the keyword: " << std::endl;
	std::cin >> temp;
	key = word;
	if (temp.length() > word.length())
	{
		for (int i = 0; i < key.length(); i++)
		{
			key[i] = temp[i];
		}
	}
	else if (temp.length() < word.length()) {
		int c = 0;
		for (int i = 0; i < key.length(); i++, c++)
		{
			key[i] = temp[c];
			if (c >= temp.size() - 1) c = -1;
		}
	}
	else
	{
		key = temp;
	}
	std::cout << "What to do with the line:\n 1 - Encrypt\n 2 - Decrypt\n";
	std::cin >> choice;
	if (choice == 1)
	{
		word = encrypt(word, key);
	}
	else if (choice == 2)
	{
		word = decrypt(word, key);
	}
	else
	{
		std::cout << "INVALID INPUT" << std::endl;
	}
	char answer = 0;
	std::cout << "Do you want to save the result to a file?[y/n]: ";
	std::cin >> answer;
	if (answer == 'y')
	{
		std::ofstream fileOut;
		fileOut.open(filename);
		if (fileOut.fail())
		{
			std::cout << "Writing failed" << std::endl;
			return 404;
		}
		fileOut << word;
		fileOut.close();
	}
	return 0;
}