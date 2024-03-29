#pragma once
#include "Tests.h"
#include "md5.h"
#include <numeric>
string chooseDirectory()
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
}
string chooseAddDirectory()
{
	vector<string> categories;
	string path = "Data\\Tests\\";
	for (const auto& a : fs::directory_iterator(path))
	{
		categories.push_back(a.path().u8string());
	}
	vector<string> show = categories;
	show.insert(show.begin(), "Add new");
	for_each(show.begin() + 1, show.end(), [](string& s) {s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1); });
	int c = Menu::select_vertical(show, HorizontalAlignment::Center, 10);
	if (c != 0)
	{
		path = categories[c - 1];
	}
	else
	{
		while (true)
		{
			system("cls");
			{
				int m = 2;
				string path = "Data\\Tests\\";
				vector<string> directories;
				for (const auto& a : fs::directory_iterator(path))
				{
					directories.push_back(a.path().u8string());
				}
				for_each(directories.begin(), directories.end(), [](string& s)
					{
						s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1);
					});
				gotoxy(2, m++);
				cout << "Already exist: " << endl;
				for_each(directories.begin(), directories.end(), [&m](string& s)
					{
						gotoxy(2, m++);
						cout << s << endl;
					});
			}
			string newDirectory;
			gotoxy(20, 13);
			cout << "Name of the new category: ";
			getline(cin, newDirectory);
			gotoxy(20, 14);
			if (!fs::exists("Data\\Tests\\" + newDirectory))
			{
				fs::create_directory("Data\\Tests\\" + newDirectory);
				path = "Data\\Tests\\" + newDirectory;
				cout << "Succesfully created" << endl;
				system("pause");
				break;
			}
			else
			{
				cout << "Such category already exists" << endl;
				system("pause");
			}
		}

	}
	path += "\\";
	return path;
}
string chooseTest()
{
	while (true)
	{
		vector<string> categories;
		vector<string> show = categories;
		string path = chooseDirectory();
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
		int c = Menu::select_vertical(show, HorizontalAlignment::Center, 10);
		path = categories[c];
		return path;
	}
}

class User abstract
{
protected:
	string path;
    public:
	User(const string path) : path{ path } {}
	virtual void menu() = 0;
};

class Guest : public User
{
public:
	Guest(const string path) noexcept : User(path)  {}
	virtual void menu() noexcept  override
	{
		system("cls");
		int c = 0;
		while (c < 4)
		{
			c = 0;
			system("cls");
			vector<string> options;
			string login;
			login.assign(this->path.begin() + this->path.find_last_of("\\") + 1, this->path.begin() + this->path.find_last_of("."));
			if (fs::exists("Data\\Unfinished Tests\\" + login + ".txt"))
			{
				options.push_back("Finish unfinished test");
			}
			else
			{
				c++;
			}
			options.push_back("Test");
			options.push_back("View personal stats");
			options.push_back("Change password");
			options.push_back("Exit");
			c += Menu::select_vertical(options, HorizontalAlignment::Left, 18);
			switch (c)
			{
			case 0:
			{
				ifstream in("Data\\Unfinished Tests\\" + login + ".txt");
				string path;
				string answers;
				getline(in, path);
				getline(in, answers);
				in.close();
				fs::remove("Data\\Unfinished Tests\\" + login + ".txt");
				Test test;
				test.constructTest(path);
				test.fill_and_play(answers,path,this->path);
			}
				break;
			case 1:
			{
				string path = chooseTest();
				path += "\\";
				Test test;
				test.constructTest(path);
				path.pop_back();
				test.play(path, this->path);
			}
				break;
			case 2:
			{
				c = 0;
				while (c < 2)
				{
					system("cls");
					c = Menu::select_vertical({ "View info in test","View info in all tests","Exit" }, HorizontalAlignment::Center, 15);
					switch (c)
					{
					case 0:
					{
						string path = chooseTest();
						string temp;
						temp.assign(path.begin() + path.find_last_of("\\") + 1, path.end());
						path = "Data\\Statistics\\" + temp + ".txt";
						system("cls");
						string login;
						login.assign(this->path.begin() + this->path.find_last_of("\\") + 1, this->path.begin() + this->path.find_last_of("."));
						ifstream in;
						string holder;
						temp.clear();
						in.open(path);
						while (getline(in, temp))
						{
							holder += temp;
							holder += '\n';
						}
						in.close();
						int m = 0;
						while (true)
						{
							if (holder.find(login, m) != string::npos)
							{
								m = holder.find(login, m);
								while (holder[m] != '-')
								{
									m--;
								}
								++m;
								while (holder[m] != '-')
								{
									cout << holder[m];
									++m;
								}
								cout << "----------------------------" << endl;
								continue;
							}
							else if (m == 0)
							{
								cout << "No records" << endl;
							}
							break;
						}
						system("pause");
					}
					break;
					case 1:
					{
						system("cls");
						gotoxy(40, 14);
						string login;
						login.assign(this->path.begin() + this->path.find_last_of("\\") + 1, this->path.begin() + this->path.find_last_of("."));
						system("cls");
						vector<string> tests;
						string path = "Data\\Statistics\\";
						for (const auto& a : fs::directory_iterator(path))
						{
							tests.push_back(a.path().u8string());
						}
						ifstream in;
						string holder;
						string temp;
						string testName;
						for_each(tests.begin(), tests.end(), [&in, &holder, &temp, &login, &testName](string& test)
							{
								testName.assign(test.begin() + test.find_last_of("\\") + 1, test.begin() + test.find_last_of("."));
								holder.clear();
								in.open(test);
								while (getline(in, temp))
								{
									holder += temp;
									holder += '\n';
								}
								int m = 0;
								temp.assign(test.begin() + test.find_last_of("\\") + 1, test.begin() + test.find_last_of("."));;
								while (true)
								{
									if (holder.find(login, m) != string::npos)
									{
										cout << "Test: " << testName;
										m = holder.find(login, m);
										while (holder[m] != '-')
										{
											m--;
										}
										++m;
										while (holder[m] != '-')
										{
											cout << holder[m];
											++m;
										}
										cout << "----------------------------" << endl;
										continue;
									}
									else if (m == 0)
									{
										cout << "No records in " << testName << endl;
									}
									break;
								}
								in.close();
							});
						system("pause");
					}
					break;
					default:
						break;
					}
				}
			}
			  break;
			case 3:
			{
				ifstream in(this->path);
				string password;
				getline(in, password);
				in.close();
				gotoxy(25, 15);
				string guess;
				int attempts = 3;
				while (attempts > 0)
				{
					guess.clear();
					system("cls");
					cout << "Old password: ";
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
						if ((a > 47 && a < 58) || (a > 64 && a < 91) || (a > 96 && a < 123)) 
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
					cout << "Incorrect password" << endl;
					system("pause");
					attempts--;
				}
				system("cls");
				if (attempts <= 0)
				{
					gotoxy(25, 16);
					cout << "Too many incorrect attempts" << endl;
					system("pause");
					break;
				}
				attempts = 2;
				while (attempts < 3)
				{
					system("cls");
					gotoxy(25, 15);
					cout << "Old password: ";
					for (size_t i = 0; i < guess.size(); i++)
					{
						cout << "*";
					}
					cout << endl;
					gotoxy(25, 16);
					cout << "New password: ";
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
						if ((a > 47 && a < 58) || (a > 64 && a < 91) || (a > 96 && a < 123))  
						{
							password += a;
							printf("*");
						}
					}
					if (password.size() < 12 && password.size() > 24)
					{
						cout << "Incorrect size of password" << endl;
						attempts--;
						continue;
					}
					break;
				}
				ofstream out(this->path);
				out << md5(password) << endl;
				out.close(); }
				break;
			default:
				break;
			}
		}
	}
};

class Admin : public User
{
public:
	Admin(const string path) noexcept : User(path) {}
	virtual void menu()noexcept override
	{
		int c = 0;
		while (c  <4)
		{
			system("cls");
			gotoxy(37, 2);
			cout << "Admin's workspace" << endl;
			c = Menu::select_vertical({"Change own credentials","Work with users","Statistics","Work with tests","Exit"}, HorizontalAlignment::Left, 18);
			switch (c)
			{
			case 0:
			{
				auto changeLogin = [&]()
				{
					string temp;
					cout << "New login: ";
					getline(cin, temp);
					string tempPath(path);
					string login;
					login.assign(tempPath.begin() + tempPath.find_last_of("\\"), temp.begin() + tempPath.find_last_of("."));
					tempPath.erase(12, login.size());
					tempPath.insert(12, temp);
					tempPath += ".txt";
					fs::rename(static_cast<const string>(path), static_cast<const string>(tempPath));
				};
				auto changePass = [&]()
				{
					while (true)
					{
						system("cls");
						gotoxy(12, 12);
						cout << "New password: ";
						string temp;
						temp = getPassword();
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
				};
			while (c < 2)
			{

				system("cls");
				c = Menu::select_vertical({ "Change login","Change password","Exit" }, HorizontalAlignment::Center);
				switch (c)
				{
				case 0:
					changeLogin();
				break;
				case 1:
					changePass();
				break;
				default:
					break;
				}

			}
			c = 0;
			}
				break;
			case 1:
			{
				auto addUser = [&]()
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
						password.clear();
						password = getPassword();
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
				};
				auto deleteUser = [&]()
				{
					string path = "Credentials\\";
					vector<string> logins;
					for (const auto& a : fs::directory_iterator(path))
					{
						logins.push_back(a.path().u8string());
					}
					if (logins.size() > 0)
					{
						vector<string> show(logins);
						for_each(show.begin(), show.end(), [](string& s) {s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1); s.erase(s.begin() + s.find_last_of("."), s.end()); });
						gotoxy(20, 8);
						cout << "Choose user to delete: ";
						c = Menu::select_vertical(show, HorizontalAlignment::Center, 9);
						fs::remove(logins[c]);
						gotoxy(20, 10);
						cout << "Successfully deleted" << endl;
					}
					else
					{
						system("cls");
						gotoxy(33, 10);
						cout << "No recorded logins" << endl;
					}
					system("pause");
				};
				while (c < 2)
				{
					system("cls");
					c = Menu::select_vertical({ "Add user","Delete user","Exit" }, HorizontalAlignment::Center);
					switch (c)
					{
					case 0:
						addUser();
						break;
					case 1:
						deleteUser();
					    break;
					default:
						break;
					}
					
				}
				c = 0;
			}
				break;
			case 2:
			{
				auto getStatsbyTest = [&]()->void
				{
					vector<string> tests;
					string path = "Data\\Statistics\\";
					for (const auto& a : fs::directory_iterator(path))
					{
						tests.push_back(a.path().u8string());
					}
					if (tests.size() == 0)
					{
						gotoxy(30, 15);
						cout << "No records" << endl;
						system("pause");
						return;
					}
					vector<string> show(tests);
					for_each(show.begin(), show.end(), [](string& s)
						{
							s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1);
							s.erase(s.begin() + s.find_last_of("."), s.end());
						});
					c = Menu::select_vertical(show, HorizontalAlignment::Center, 10);
					path = tests[c];
					ifstream in;
					string temp;
					in.open(path);
					while (getline(in, temp))
					{
						cout << temp << endl;
					}
					in.close();
					system("pause");
				};
				auto getStatsbyUser = [&]()->void
				{
					system("cls");
					gotoxy(20, 14);
					string login;
					cout << "Login of the user whose stats to show: ";
					getline(cin, login);
					if (!fs::exists("Credentials\\" + login + ".txt") && !fs::exists("Data\\Admins\\" + login + ".txt"))
					{
						gotoxy(20, 15);
						cout << "No such login exists" << endl;
					}
					else
					{
						system("cls");
						vector<string> tests;
						string path = "Data\\Statistics\\";
						for (const auto& a : fs::directory_iterator(path))
						{
							tests.push_back(a.path().u8string());
						}
						if (tests.size() == 0)
						{
							gotoxy(20, 15);
							cout << "No records" << endl;
							system("pause");
							return;
						}
						ifstream in;
						string holder;
						string temp;
						string testName;
						for_each(tests.begin(), tests.end(), [&in, &holder, &temp, &login, &testName](string& test)
							{
								testName.assign(test.begin() + test.find_last_of("\\") + 1, test.begin() + test.find_last_of("."));
								holder.clear();
								in.open(test);
								while (getline(in, temp))
								{
									holder += temp;
									holder += '\n';
								}
								int m = 0;
								temp.assign(test.begin() + test.find_last_of("\\") + 1, test.begin() + test.find_last_of("."));;
								while (true)
								{
									if (holder.find(login, m) != string::npos)
									{
										cout << "Test: " << testName;
										m = holder.find(login, m);
										while (m > 0 && holder[m] != '-')
										{
											m--;
										}
										++m;
										while (holder[m] != '-')
										{
											cout << holder[m];
											++m;
										}
										cout << "----------------------------" << endl;
										continue;
									}
									break;
								}
								in.close();
							});
					}
					system("pause");
				};
				auto getStatsOverall = [&]()->void
				{
					system("cls");
					vector<string> tests;
					for (const auto& a : fs::directory_iterator("Data\\Statistics"))
					{
						tests.push_back(a.path().u8string());
					}
					if (tests.size() == 0)
					{
						gotoxy(20, 15);
						cout << "No records" << endl;
						system("pause");
						return;
					}
					int entries = 0;
					double timeOverall = 0;
					int correctness = 0;
					ifstream in;
					string temp;
					for_each(tests.begin(), tests.end(), [&](string& s)
						{
							in.open(s);
							while (getline(in, temp))
							{
								if (temp._Starts_with("--------"))
								{
									entries++;
								}
								else if (temp._Starts_with("Time spent:"))
								{
									int ind = 12;
									string time;
									while (temp[ind] != '\0')
									{
										time += temp[ind++];
									}
									timeOverall += stod(time);
								}
								else if (temp._Starts_with("Correct answers: "))
								{
									int ind = strlen("Correct answers: ");
									string percent;
									while (temp[ind] != ' ')
									{
										percent += temp[ind++];
									}
									correctness += stoi(percent);
								}
							}
							in.close();
						});
					int m = 13;
					gotoxy(30, m++);
					cout << "Entries: " << entries << endl;
					gotoxy(30, m++);
					cout << "Time spent overall: " << timeOverall << endl;
					gotoxy(30, m++);
					cout << "Average grade: " << ((double)correctness / double(100 * entries)) * 12 << endl;
					system("pause");
				};
				while (c < 3)
				{
					system("cls");
					c = Menu::select_vertical({ "Stats by test","Stats by user","Stats overall","Exit" }, HorizontalAlignment::Center, 10);
					switch (c)
					{
					case 0:
						getStatsbyTest();
					break;
					case 1:
						getStatsbyUser();
					break;
					case 2:
						getStatsOverall();
					break;
					default:
						break;
					}
					
				}
				c = 0;
			}
				break;
			case 3:
			{
				auto addCategory = [&]()
				{
					system("cls");
					{
						int m = 2;
						string path = "Data\\Tests\\";
						vector<string> directories;
						for (const auto& a : fs::directory_iterator(path))
						{
							directories.push_back(a.path().u8string());
						}
						for_each(directories.begin(), directories.end(), [](string& s)
							{
								s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1);
							});
						gotoxy(2, m++);
						cout << "Already exist: " << endl;
						for_each(directories.begin(), directories.end(), [&m](string& s)
							{
								gotoxy(2, m++);
								cout << s << endl;
							});
					}
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
				};
				auto addTest = [&]()
				{
					system("cls");
					string path = chooseAddDirectory();
					system("cls");
					string newTest;
					gotoxy(20, 13);

					{
						int m = 2;
						vector<string> directories;
						for (const auto& a : fs::directory_iterator(path))
						{
							directories.push_back(a.path().u8string());
						}
						for_each(directories.begin(), directories.end(), [](string& s)
							{
								s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1);
							});
						gotoxy(2, m++);
						cout << "Already exist: " << endl;
						for_each(directories.begin(), directories.end(), [&m](string& s)
							{
								gotoxy(2, m++);
								cout << s << endl;
							});
					}

					gotoxy(20, 14);
					cout << "Name of the new test: ";
					getline(cin, newTest);
					gotoxy(20, 15);
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
				};
				auto deleteCategory = [&]()
				{
					string path = chooseDirectory();
					path.pop_back();
					fs::remove(path);
				};
				auto deleteTest = [&]()
				{
					string path = chooseTest();
					fs::remove(path);
				};
				auto changeTest = [&]() {};
				auto tryTest = [&]() 
				{
					string path = chooseTest();
					path += "\\";
					Test test;
					test.constructTest(path);
					path.pop_back();
					test.play(path, this->path);
				};
				while (c < 6)
				{
					system("cls");
					 c= Menu::select_vertical({ "Add category","Add test to category","Delete test from category","Change test","Delete category","Try test","Exit" }, HorizontalAlignment::Left, 18);
					switch (c)
					{
					case 0:
						addCategory();
						break;
					case 1:
						addTest();
						break;
					case 2:
						deleteTest();
						break;
					case 3:
					{
						string path = chooseTest();
						path += "\\";
						auto addQuest = [&]()
						{
							system("cls");
							Question temp;
							{
								int m = 10;
								gotoxy(20, m++);
								cout << "Question: ";
								getline(cin, temp.question);
								int ind = 1;
								while (c < 1)
								{
									system("cls");
									int cur = 5;
									ind = 1;
									gotoxy(15, cur++);
									cout << temp.question << endl;
									for_each(temp.answers.begin(), temp.answers.end(), [&cur, &ind](string& s) {gotoxy(15, cur++); cout << ind++ << ".\t" << s << endl; });
									if (temp.answers.size() < 2)
									{
										gotoxy(20, m);
										cout << "Answer: ";
										string answer;
										getline(cin, answer);
										temp.answers.push_back(answer);
									}
									else
									{
										gotoxy(30, 15);
										cout << "Add another answer?" << endl;
										c = Menu::select_vertical({ "Yes","No" }, HorizontalAlignment::Center, 16);
										switch (c)
										{
										case 0:
										{
											cur = 5;
											ind = 1;
											gotoxy(15, cur++);
											cout << temp.question << endl;
											for_each(temp.answers.begin(), temp.answers.end(), [&cur, &ind](string& s) {gotoxy(15, cur++); cout << ind++ << ".\t" << s << endl; });
											gotoxy(20, m++);
											cout << "Answer: ";
											string answer;
											getline(cin, answer);
											temp.answers.push_back(answer);
										}
										break;
										default:
											break;
										}
									}

								}
								system("cls");
								m = 10;
								ind = 1;
								for_each(temp.answers.begin(), temp.answers.end(), [&m, &ind](string answer) {gotoxy(25, m++); cout << ind++ << ".\t" << answer << endl; });
								gotoxy(20, m++);
								cout << "Index of the correct answer: ";
								cin >> ind;
								if (isdigit(ind))
								{
									temp.correct = --ind;
								}
								else
								{
									temp.correct = 0;
								}
								cin.ignore();
							}
							string lastFile;
							{
								vector<string> questions;
								for (const auto& newQuestion : fs::directory_iterator(path))
								{
									questions.push_back(newQuestion.path().u8string());
								}
								if (questions.size() == 0)
								{
									questions.push_back(path += "0.txt");
								}
								sort(questions.begin(), questions.end(), [](string& s1, string& s2) {return std::accumulate(s1.begin(), s1.end(), 0) < std::accumulate(s2.begin(), s2.end(), 0); });
								lastFile = questions.back();
							}
							string newFile;
							newFile.assign(lastFile.begin() + lastFile.find_last_of("\\") + 1, lastFile.begin() + lastFile.find_last_of("."));
							newFile = to_string(stoi(newFile) + 1);
							size_t ind = lastFile.find_last_of("\\") + 1;
							for (size_t i = 0; i < newFile.size(); i++)
							{

								if (i > 0)
								{
									lastFile.insert(lastFile.begin() + ind, newFile[i]);
								}
								else
								{
									lastFile[ind] = newFile[i];
								}
								++ind;
							}
							ofstream out(lastFile);
							out << temp.question << "\n";
							out << temp.correct << "\n";
							for_each(temp.answers.begin(), temp.answers.end(), [&out](string s) {out << s << endl; });
							out.close();

						};
						auto changeQuest = [&]()
						{
							system("cls");
							vector<string> questions;
							for (const auto& a : fs::directory_iterator(path))
							{
								questions.push_back(a.path().u8string());
							}
							sort(questions.begin(), questions.end(), [](string& s1, string& s2) {return std::accumulate(s1.begin(), s1.end(), 0) < std::accumulate(s2.begin(), s2.end(), 0); });
							vector<string> show(questions);
							for_each(show.begin(), show.end(), [](string& str) {str.erase(str.begin(), str.begin() + str.find_last_of("\\") + 1); });
							for_each(show.begin(), show.end(), [](string& str) {str.erase(str.begin() + str.find_first_of("."), str.end()); });
							int g = Menu::select_vertical(show, HorizontalAlignment::Center);
							ifstream in(questions[g]);
							Question temp;
							getline(in, temp.question);
							in >> temp.correct;
							{
								string trash;
								getline(in, trash);
								while (getline(in, trash))
								{
									temp.answers.push_back(trash);
								}
							}
							in.close();
							while (c < 5)
							{
								system("cls");
								gotoxy(25, 8);
								cout << temp.question << endl;
								gotoxy(25, 9);
								cout << "Correct answer: " << temp.correct + 1 << endl;
								int m = 10;
								for_each(temp.answers.begin(), temp.answers.end(), [&m](string& answer) {gotoxy(25, m++); cout << answer << endl; });
								c = Menu::select_vertical({ "Change question","Change correct answer","Change answer","Add answer","Delete answer","Exit" }, HorizontalAlignment::Left, 16);
								switch (c)
								{
								case 0:
									system("cls");
									gotoxy(25, 10);
									cout << "New question: ";
									getline(cin, temp.question);
									break;
								case 1:
									while (true)
									{
										system("cls");
										m = 10;
										int ind = 1;
										for_each(temp.answers.begin(), temp.answers.end(), [&m, &ind](string& answer) {gotoxy(25, m++); cout << ind++ << ".   " << answer << endl; });
										gotoxy(25, m);
										cout << "Correct answer: ";
										cin >> temp.correct;
										cin.ignore();
										--temp.correct;
										if (temp.correct >= 0 && temp.correct < temp.answers.size())
										{
											break;
										}
										cout << "Incorrect index" << endl;
										system("pause");
									}
									break;
								case 2:
								{
									system("cls");
									m = 10;
									int ind = 1;
									for_each(temp.answers.begin(), temp.answers.end(), [&](string& answer) {gotoxy(25, m++); cout << ind++ << ".\t" << answer << endl; });
									gotoxy(25, m++);
									cout << "Answer to change: ";
									cin >> ind;
									cin.ignore();
									if (--ind >= 0 && ind < temp.answers.size())
									{
										string newAnswer;
										gotoxy(25, m++);
										cout << "New answer: ";
										cin.ignore();
										getline(cin, newAnswer);
										temp.answers[ind] = newAnswer;
										gotoxy(25, m);
										cout << "Succesfully changed" << endl;
									}
									else
									{
										gotoxy(25, m);
										cout << "Inccorrect index" << endl;
									}
									system("pause"); }
								break;
								case 3:
								{
									system("cls");
									gotoxy(25, 10);
									cout << "New answer: ";
									string newAnswer;
									getline(cin, newAnswer);
									temp.answers.push_back(newAnswer);
								}
								break;
								case 4:
								{
									system("cls");
									m = 10;
									int ind = 1;
									for_each(temp.answers.begin(), temp.answers.end(), [&](string& answer) {gotoxy(25, m++); cout << ind++ << ".\t" << answer << endl; });
									gotoxy(25, m++);
									cout << "Answer to delete: ";
									cin >> ind;
									cin.ignore();
									if (--ind >= 0 && ind < temp.answers.size())
									{
										temp.answers.erase(temp.answers.begin() + ind);
										gotoxy(25, m++);
										cout << "Successfully deleted" << endl;
									}
									else
									{
										gotoxy(25, m++);
										cout << "Invalid index" << endl;
									}
									system("pause");
								}
								break;
								default:
									break;
								}
							}
							ofstream out(questions[g]);
							out << temp.question << "\n";
							out << temp.correct << "\n";
							for_each(temp.answers.begin(), temp.answers.end(), [&](string& s) {out << s << "\n"; });
							out.close();
						};
						auto viewQuest = [&]()
						{

							vector<string> questions;

							for (const auto& a : fs::directory_iterator(path))
							{
								questions.push_back(a.path().u8string());
							}
							sort(questions.begin(), questions.end(), [](string& s1, string& s2) {return std::accumulate(s1.begin(), s1.end(), 0) < std::accumulate(s2.begin(), s2.end(), 0); });

							vector<string> show(questions);
							for_each(show.begin(), show.end(), [](string& str) {str.erase(str.begin(), str.begin() + str.find_last_of("\\") + 1); });
							for_each(show.begin(), show.end(), [](string& str) {str.erase(str.begin() + str.find_first_of("."), str.end()); });

							{
								ifstream in;
								string quest;
								int ind = 0;
								for_each(show.begin(), show.end(), [&questions, &in, &quest, &ind](string& s)
									{
										in.open(questions[ind++]);
										getline(in, quest);
										in.close();
										s += ".   " + quest;
									});
							}

							c = Menu::select_vertical(show, HorizontalAlignment::Center);

							ifstream in(questions[c]);
							string temp;
							system("cls");
							while (getline(in, temp))
							{
								cout << temp << endl;
							}
							system("pause");
						};
						auto deleteQuest = [&]()
						{
							ifstream in;
							string quest;
							int ind = 0;
							string last;

							{
								vector<string> questions;
								for (const auto a : fs::directory_iterator(path))
								{
									questions.push_back(a.path().u8string());
								}
								sort(questions.begin(), questions.end(), [](string& s1, string& s2) {return std::accumulate(s1.begin(), s1.end(), 0) < std::accumulate(s2.begin(), s2.end(), 0); });
								for_each(questions.begin(), questions.end(), [&](string& s)
									{
										in.open(s);
										getline(in, quest);
										cout << ++ind << ".  " << quest << endl;
										in.close();
									});
							}

							int number;
							cout << "Number of the question you want to delete: ";
							cin >> number;
							cin.ignore();
							if (fs::exists(path + to_string(number) + ".txt"))
							{
								fs::remove(path + to_string(number) + ".txt");
								cout << "Succesfully deleted" << endl;
								while (fs::exists(path + to_string(number + 1) + ".txt"))
								{
									fs::rename(path + to_string(number + 1) + ".txt", path + to_string(number++) + ".txt");
								}
							}
							else
							{
								cout << "No such number exists" << endl;
							}
							cin.ignore();
							system("pause");
						};
						while (c < 4)
						{
							system("cls");
							c = Menu::select_vertical({ "Add question","Change question","View Question","Delete question","Exit"}, HorizontalAlignment::Center, 14);
							switch (c)
							{
							case 0:
								addQuest();
								break;
							case 1:
								changeQuest();
								break;
							case 2:
								viewQuest();
								break;
							case 3:
								deleteQuest();
							   break;
							default:
								break;
							}
						}

					}
						break;
					case 4:
						deleteCategory();
						break;
					case 5:
						tryTest();
						break;
					default:
						break;
					}
					
				}
				c = 0;
			}
				break;
			default:
				break;
			}
		}
	}
};