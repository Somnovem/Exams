#pragma once
#include<iostream>
#include <fstream>
#include<experimental/filesystem>
#include "../Library/Menu.h"
#include "../Library/Functions.h"
#include "Tests.h"
#include "md5.h"
using namespace std;
namespace fs = std::experimental::filesystem;
class User abstract
{
protected:
	string name;
	int age = 0;
	string phone;
	string adress;
	string path;
    public:
	User(const string path) : path{ path } {}
	virtual void menu() = 0;
};

class Guest : public User
{
public:
	Guest(const string path) : User(path) {}
	virtual void menu() noexcept override
	{
		cout << "Guest" << endl;
	}
};

class Admin : public User
{
public:
	Admin(const string path) : User(path) {}
	
	virtual void menu()noexcept override
	{
		int c = 0;
		while (c  <4)
		{
			system("cls");
			 c = Menu::select_vertical({"Change own credentials","Work with users","Statistics","Work with tests","Exit"}, HorizontalAlignment::Left, 18);
			switch (c)
			{
			case 0:
				while (c < 2)
				{
					system("cls");
					c = Menu::select_vertical({ "Change login","Change password","Exit" }, HorizontalAlignment::Center);
					switch (c)
					{
					case 0:
					{
						string temp;
						cout << "New login: ";
						getline(cin, temp);
						string tempPath(path);
						string login(tempPath.substr(12, tempPath.find_last_of("\\") - 12));
						tempPath.erase(12, login.size());
						tempPath.insert(12, temp);
						tempPath += ".txt";
						fs::rename(static_cast<const string>(path), static_cast<const string>(tempPath));
					}
					break;
					case 1:
					{
						while (true)
						{
							system("cls");
							gotoxy(12, 12);
							cout << "New password: ";
							string temp;
							getline(cin, temp);
							if (temp.size() < 12 || temp.size() > 24)
							{
								gotoxy(12, 13);
								cout << "Incorrect size" << endl;
								system("pause");
								continue;
							}
							ofstream out(path);
							out << md5(temp);
							out.close();
							break;
						}
					}
					break;
					default:
						break;
					}
				}
				break;
			case 1:
			{
				while (c < 3)
				{
					system("cls");
					c = Menu::select_vertical({ "Add user","Delete user","Change user","Exit" }, HorizontalAlignment::Center);
					switch (c)
					{
					case 0:
					{
						string login;
						string password;
						while (true)
						{
							system("cls");
							cout << "Login: ";
							getline(cin, login);
							if (fs::exists("Credentials\\" + login + ".txt"))
							{
								cout << "Already exists" << endl;
								system("pause");
								continue;
							}
							break;
						}
						while (true)
						{
							system("cls");
							cout << "Login: " << login << endl;
							cout << "Password(from 12 to 24 symbols): ";
							getline(cin, password);
							if (password.length() < 12 || password.length() > 24)
							{
								cout << "Incorrect length of password" << endl;
								system("pause");
								continue;
							}
							break;
						}
						ofstream out;
						out.open("Credentials\\" + login + ".txt");
						out << md5(password) << endl;
						out.close();
					}
						break;
					case 1:
					{
						string login;
						cout << "Login of the user you want to delete: ";
						getline(cin, login);
						if (fs::exists("Credentials\\" + login + ".txt"))
						{
							fs::remove("Credentials\\" + login + ".txt");
							cout << "Successfully deleted" << endl;
						}
						else
						{
							cout << "No such login exists" << endl;
						}
						system("pause");
					}
					 break;
					case 2:
					{
						string oldLogin;
						string newLogin;
						cout << "Login you want to change: ";
						getline(cin, oldLogin);
						if (fs::exists("Credentials\\" + oldLogin + ".txt"))
						{
							cout << "New login: ";
							getline(cin, newLogin);
							fs::rename("Credentials\\" + oldLogin + ".txt", "Credentials\\" + newLogin + ".txt");

						}
					}
						break;
					default:
						break;
					}
				}
			}
				break;
			case 2:
			{
			}
				break;
			case 3:
			{
				int c = 0;
				auto chooseDirectory = []()->string
				{
					while (true)
					{
						vector<string> categories;
						string path = "Data\\Tests\\";
						for (const auto& a : fs::directory_iterator(path))
						{
							categories.push_back(a.path().u8string());
						}
						if (categories.size() == 0)
						{
							gotoxy(20, 14);
							cout << "No records" << endl;
							continue;
						}
						vector<string> show = categories;
						for_each(show.begin(), show.end(), [](string& s) {s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1); });
						int c = Menu::select_vertical(show, HorizontalAlignment::Center, 10);
						path = categories[c];
						path += "\\";
						return path;
					}
					
				};
				auto chooseTest = []()->string
				{
					while (true)
					{
						vector<string> categories;
						string path = "Data\\Tests\\";
						for (const auto& a : fs::directory_iterator(path))
						{
							categories.push_back(a.path().u8string());
						}
						if (categories.size() == 0)
						{
							gotoxy(20, 14);
							cout << "No records" << endl;
							continue;
						}
						vector<string> show = categories;
						for_each(show.begin(), show.end(), [](string& s) {s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1); });
						int c = Menu::select_vertical(show, HorizontalAlignment::Center, 10);
						path = categories[c];
						path += "\\";
						show.clear();
						categories.clear();
						for (const auto& a : fs::directory_iterator(path))
						{
							categories.push_back(a.path().u8string());
						}
						if (categories.size() == 0)
						{
							gotoxy(20, 14);
							cout << "No records" << endl;
							continue;
						}
						show = categories;
						for_each(show.begin(), show.end(), [](string& s) {s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1); });
						c = Menu::select_vertical(show, HorizontalAlignment::Center, 10);
						path = categories[c];
						return path;
					}
				};
				while (c <= 5)
				{
					system("cls");
					 c= Menu::select_vertical({ "Add category","Add test to category","Delete test from category","Change test","Delete category","Try test","Exit" }, HorizontalAlignment::Left, 18);
					switch (c)
					{
					case 0:
					{
						string newDirectory;
						gotoxy(20, 13);
						cout << "Name of the new category: ";
						getline(cin, newDirectory);
						gotoxy(20, 14);
						if (!fs::exists("Data\\Tests\\" + newDirectory))
						{
							fs::create_directory("Data\\Tests\\" + newDirectory);
							cout << "Succesfully created" << endl;
						}
						else
						{
							cout << "Such category already exists" << endl;
						}
						system("pause");
					}
						break;
					case 1:
					{
						string path = chooseDirectory();
						string newTest;
						gotoxy(20, 13);
						cout << "Name of the new test: ";
						getline(cin, newTest);
						gotoxy(20, 14);
						if (!fs::exists(path + newTest))
						{
							fs::create_directory(path + newTest);
							cout << "Succesfully created" << endl;
						}
						else
						{
							cout << "Such category already exists" << endl;
						}
						system("pause");
					}
						break;
					case 2:
					{
						string path = chooseTest();
						fs::remove(path);
					}
						break;
					case 3:
					{
						string path = chooseTest();
						path += "\\";
					}
						break;
					case 4:
					{
						string path = chooseDirectory();
						fs::remove(path);
					}
						break;
					case 5:
					{
						string path = chooseTest();
						path += "\\";
						Test test;
						test.constructTest(path);
						test.play();
					}
						break;
					default:
						break;
					}
				}
			}
				break;
			default:
				break;
			}
		}
	}
};

