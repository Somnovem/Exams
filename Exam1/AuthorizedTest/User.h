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
				auto chooseTest = [&]()->string
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

						while (c < 4)
						{
							system("cls");
							c = Menu::select_vertical({ "Add question","Change question","View Question","Delete question","Exit"}, HorizontalAlignment::Center, 14);
							switch (c)
							{
							case 0:
							{
								system("cls");
								Question temp;
								{
									int m = 10;
									gotoxy(50, m++);
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
											gotoxy(50, m);
											cout << "Answer: ";
											string answer;
											getline(cin, answer);
											temp.answers.push_back(answer);
										}
										else
										{
											gotoxy(30, 10);
											cout << "Add another answer?" << endl;
											c = Menu::select_vertical({ "Yes","No" }, HorizontalAlignment::Center);
											switch (c)
											{
											case 0:
											{
												cur = 5;
												ind = 1;
												gotoxy(15, cur++);
												cout << temp.question << endl;
												for_each(temp.answers.begin(), temp.answers.end(), [&cur, &ind](string& s) {gotoxy(15, cur++); cout << ind++ << ".\t" << s << endl; });
												gotoxy(50, m++);
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
									for_each(temp.answers.begin(), temp.answers.end(), [&m,&ind](string answer) {gotoxy(50, m++); cout << ind++ << ".\t" << answer << endl; });
									gotoxy(50, m++);
									cout << "Index of the correct answer: ";
									cin >> ind;
									temp.correct = --ind;
									cin.ignore();
								}
								string lastFile;
								for (const auto& newQuestion : fs::directory_iterator(path))
								{
									lastFile = newQuestion.path().u8string();
								}
								string newFile;
								newFile.assign(lastFile.begin() + lastFile.find_last_of("\\") + 1, lastFile.begin() + lastFile.find_last_of("."));
								newFile = to_string(stoi(newFile) + 1);
								size_t ind = lastFile.find_last_of("\\") + 1;
								for (size_t i = 0; i < newFile.size(); i++)
								{
									lastFile[ind++] = newFile[i];
								}
								ofstream out(lastFile);
								out << temp.question << "\n";
								out << temp.correct << "\n";
								for_each(temp.answers.begin(), temp.answers.end(), [&out](string s) {out << s << endl; });
								out.close();

							}
								break;
							case 1:
							{
								system("cls");
								vector<string> questions;
								for (const auto& a : fs::directory_iterator(path))
								{
									questions.push_back(a.path().u8string());
								}
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
									while (getline(in,trash))
									{
										temp.answers.push_back(trash);
									}
								}
								in.close();
								while (c < 5)
								{
									system("cls");
									gotoxy(50, 8);
									cout << temp.question << endl;
									gotoxy(50, 9);
									cout << "Correct answer: " << temp.correct + 1 << endl;
									int m = 10;
									for_each(temp.answers.begin(), temp.answers.end(), [&m](string& answer) {gotoxy(50, m++); cout << answer << endl; });
									c = Menu::select_vertical({"Change question","Change correct answer","Change answer","Add answer","Delete answer","Exit"}, HorizontalAlignment::Left, 16);
									switch (c)
									{
									case 0:
										system("cls");
										gotoxy(50, 10);
										cout << "New question: ";
										getline(cin, temp.question);
										break;
									case 1:
										while (true)
										{
											system("cls");
											m = 10;
											for_each(temp.answers.begin(), temp.answers.end(), [&m](string& answer) {gotoxy(50, m++); cout << answer << endl; });
											gotoxy(50, m);
											cout << "Correct answer: ";
											cin >> temp.correct;
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
									for_each(temp.answers.begin(), temp.answers.end(), [&](string& answer) {gotoxy(50, m++); cout << ind++<<".\t" << answer << endl; });
									gotoxy(50, m++);
									cout << "Answer to change: ";
									cin >> ind;
									if (--ind >= 0 && ind < temp.answers.size())
									{
										string newAnswer;
										gotoxy(50, m++);
										cout << "New answer: ";
										cin.ignore();
										getline(cin, newAnswer);
										temp.answers[ind] = newAnswer;
										gotoxy(50, m);
										cout << "Succesfully changed" << endl;
									}
									else
									{
										gotoxy(50, m);
										cout << "Inccorrect index" << endl;
									}
									system("pause"); }
										break;
									case 3:
									{
										system("cls");
										gotoxy(50, 10);
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
										for_each(temp.answers.begin(), temp.answers.end(), [&](string& answer) {gotoxy(50, m++); cout << ind++ << ".\t" << answer << endl; });
										gotoxy(50, m++);
										cout << "Answer to delete: ";
										cin >> ind;
										if (--ind >= 0 && ind < temp.answers.size())
										{
											temp.answers.erase(temp.answers.begin() + ind);
											gotoxy(50, m++);
											cout << "Successfully deleted" << endl;
										}
										else
										{
											gotoxy(50, m++);
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
							}
								break;
							case 2:
							{

								vector<string> questions;
								for (const auto& a : fs::directory_iterator(path))
								{
									questions.push_back(a.path().u8string());
								}
								vector<string> show(questions);
								for_each(show.begin(), show.end(), [](string& str) {str.erase(str.begin(),str.begin()+ str.find_last_of("\\") + 1); });
								for_each(show.begin(), show.end(), [](string& str) {str.erase(str.begin() + str.find_first_of("."),str.end()); });
								c = Menu::select_vertical(show, HorizontalAlignment::Center);
								ifstream in(questions[c]);
								string temp;
								system("cls");
								while (getline(in,temp))
								{
									cout << temp << endl;
								}
								system("pause");
							}
								break;
							case 3:
							{
								ifstream in;
								string quest;
								int ind = 0;
								string last;
								for (const auto a : fs::directory_iterator(path))
								{
									last = a.path().u8string();
									in.open(a.path());
									getline(in, quest);
									cout << ++ind <<".  " << quest << endl;
									in.close();
								}
								int number;
								cout << "Number of the question you want to delete: ";
								cin >> number;
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
							}
							   break;
							default:
								break;
							}
						}

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

