#pragma once
#include "User.h"
#include <windows.h>
class Application
{
	User* user = nullptr;
	void reg()
	{
		std::string login;
		std::string password;
		if (!fs::exists("Credentials"))
		{
			fs::create_directory("Credentials");
		}
		if (!fs::exists("Data"))
		{
			fs::create_directory("Data");
		}
		if (!fs::exists("Data\\Admins"))
		{
			fs::create_directory("Data\\Admins");
		}
		if (!fs::exists("Data\\Tests"))
		{
			fs::create_directory("Data\\Tests");
		}
		if (!fs::exists("Data\\Statistics"))
		{
			fs::create_directory("Data\\Statistics");
		}
		//getting the login
		while (true)
		{
			system("cls");
			if (fs::is_empty("Data\\Admins"))
			{
				cout << "First register. You shall be the admin" << endl;
			}
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
		//getting the password
		while (true)
		{
			system("cls");
			cout << "Login: " << login << endl;
			cout << "Password(from 12 to 24 symbols): ";
			password.clear();
			while (true)
			{
				char a = _getch();
				if (a == 13)
				{
					break;
				}
				else if (a == '\b')
				{
					if (password.size() > 0)
					{
						password.pop_back();
						cout << "\b \b";
					}
				}
				if ((a > 47 && a < 58) || (a > 64 && a < 91) || (a > 96 && a < 123))  //ASCii code for integer and alphabet
				{
					password += a;
					printf("*");
				}
			}
			if (password.length() < 12 || password.length() > 24)
			{
				cout << "Incorrect length of password" << endl;
				system("pause");
				continue;
			}
			break;
		}
		ofstream out;
		if (fs::is_empty("Data\\Admins"))
		{
			out.open("Data\\Admins\\" + login + ".txt");
			user = new Admin("Data\\Admins\\" + login + ".txt");
		}
		else
		{
			out.open("Credentials\\" + login + ".txt");
			user = new Guest("Credentials\\" + login + ".txt");
		}
		out << md5(password) << endl;
		out.close();
		user->menu();
	}
	void log()
	{
		system("cls");
		if ((!fs::exists("Credentials")
			|| fs::is_empty("Credentials"))
			&& (!fs::exists("Data\\Admins")
			|| fs::is_empty("Data\\Admins")))
		{
			gotoxy(30, 15);
			cout << "No recorded logins" << endl;
			return;
		}
		std::string login;
		std::string path;
		while (true)
		{
			system("cls");
			cout << "Login: ";
			getline(cin, login);
			if (fs::exists("Data\\Admins\\" + login + ".txt"))
			{
				path = "Data\\Admins\\" + login + ".txt";
				user = new Admin(path);
				break;
			}
			if (!fs::exists("Credentials\\" + login + ".txt"))
			{
				cout << "No such login exists" << endl;
				system("pause");
				continue;
			}
			path = "Credentials\\" + login + ".txt";
			user = new Guest(path);
			break;
		}
		ifstream in(path);
		std::string password;
		getline(in, password);
		in.close();
		int attempts = 3;
		while (attempts > 0)
		{
			system("cls");
			cout << "Login: " << login << endl;
			cout << "Password: ";
			std::string guess;
			guess.clear();
			while (true)
			{
				char a = _getch();
				if (a == 13)
				{
					break;
				}
				else if (a == '\b')
				{
					if (guess.size() > 0)
					{
						guess.pop_back();
						cout << "\b \b";
					}
				}
				if ((a > 47 && a < 58) || (a > 64 && a < 91) || (a > 96 && a < 123))  //ASCii code for integer and alphabet
				{
					guess += a;
					printf("*");
				}
			}
			cout << endl;
			if (md5(guess) == password)
			{
				break;
			}
			else
			{
				cout << "Incorrect password" << endl;
				system("pause");
			}
		}
		if (attempts <= 0)
		{
			cout << "Too many inccorect attempts" << endl;
			system("pause");
			return;
		}
		user->menu();
	}
public:
	Application() {}
	void menu()
	{
		while (true)
		{
			HWND console = GetConsoleWindow();
			RECT r;
			GetWindowRect(console, &r);
			MoveWindow(console, r.left, r.top, 1000, 700, TRUE);
			system("cls");
			int c = Menu::select_vertical({ "Register","Login","Exit" }, HorizontalAlignment::Center);
			switch (c)
			{
			case 0:
				reg();
				break;
			case 1:
				log();
				break;
			case 2:
				exit(0);
			}
			delete user;
			user = nullptr;
		}
	}
};


