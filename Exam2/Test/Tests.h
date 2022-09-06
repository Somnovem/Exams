#pragma once
#include <iostream>
#include <vector>
#include "../Library/Menu.h"
#include "../Library/Functions.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <experimental/filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;
class Question
{
public:
	string question;
	vector<string> answers;
	size_t correct = 0;
	Question()noexcept {}
	Question(const Question& _Other)noexcept
	{
		this->question = _Other.question;
		this->answers = _Other.answers;
		this->correct = _Other.correct;
	}
	Question& operator=(const Question& _Other)noexcept
	{
		if (&_Other == this)
		{
			return *this;
		}
		if (this != nullptr)
		{
			delete this;
		}
		this->question = _Other.question;
		this->answers = _Other.answers;
		this->correct = _Other.correct;
	}
	int pass()noexcept
	{
		answers.push_back("Exit");
		while (true)
		{
			system("cls");
			gotoxy(20, 6);
			cout << question << endl;
			int c = Menu::select_vertical(answers, HorizontalAlignment::Center, 7);
			if (c == answers.size() - 1)
			{
				system("cls");
				gotoxy(20, 14);
				cout << "If you have an unfinished test,it will be overwritten" << endl;
				c = Menu::select_vertical({ "Yes(Exit)", "No(Go back)" }, HorizontalAlignment::Center, 15);
				if (c == 0)
				{
					return 2;
				}
				continue;
			}
			else if (c == correct)
			{
				return 1;
			}
			return 0;
		}
	}
};
class Test
{
	vector<Question> test;
	vector<int> correctness;
public:
	void constructTest(string path)noexcept
	{
		vector<string> quests;
		for (const auto& file : fs::directory_iterator(path))
		{
			quests.push_back(file.path().u8string());
		}
		ifstream in;
		for (size_t i = 0; i < quests.size(); i++)
		{
			in.open(quests[i]);
			Question temp;
			getline(in, temp.question);
			in >> temp.correct;
			string answer;
			getline(in, answer);
			while (getline(in,answer))
			{
				temp.answers.push_back(answer);
			}
			test.push_back(temp);
			in.close();
		}
	}
	void play(const string& pathToTest,const string& user, size_t from = 0)noexcept
	{
		if (!fs::exists("Data\\Unfinished Tests"))
		{
			fs::create_directory("Data\\Unfinished Tests");
		}

		auto start = std::chrono::system_clock::now();
		int buff = 0;
		string login;
		login.assign(user.begin() + user.find_last_of("\\") + 1, user.begin() + user.find_last_of("."));
		bool exited = false;

		for_each(test.begin() + from, test.end(), [&](Question temp)
		{
				if (!exited)
				{
					buff = temp.pass();
					if (buff < 2)
					{
						correctness.push_back(buff);
					}
					else
					{
						ofstream out("Data\\Unfinished Tests\\" + login + ".txt");
						out << pathToTest << endl;
						for_each(correctness.begin(), correctness.end(), [&out](int n) {out << to_string(n); });
						out << endl;
						out.close();
						exited = true;
					}
				}
				
		});
		if (exited)
		{
			return;
		}

		auto end = std::chrono::system_clock::now();

		system("cls");
		size_t correct = count(correctness.begin(), correctness.end(), 1);
		double percent = (static_cast<double>(correct) / test.size()) * 100;
		int m = 13;

		gotoxy(20, m++);
		cout << "Grade: " << double(correct)/ double(test.size()) * 12 << endl;
		gotoxy(20, m++);
		cout << "Percentage:  " << percent << "%" << endl;
		cout.precision(2);
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		if (correct != test.size())
		{
			auto findMistakes = [&]()
			{
				int i = 1;
				for_each(correctness.begin(), correctness.end(), [&i](bool answer) {
					if (!answer)
						cout << i << " ";
					i++; });
			};
			gotoxy(20, m++);
			cout << "Mistakes in questions number: ";
			findMistakes();
			cout << endl;
		}
		gotoxy(20, m++);
		cout << "Time spent: " << elapsed_seconds.count() << " s" << endl;
		gotoxy(20, m++);
		cout << "Finished at: " << std::ctime(&end_time);

		string testName;
		testName.assign(pathToTest.begin() + pathToTest.find_last_of("\\") + 1, pathToTest.end());
		
		ofstream out("Data\\Statistics\\" + testName  + ".txt",ios::app);
		out << "Time spent: " << elapsed_seconds.count() << " s" << endl;
		out << "Finished at: " <<  std::ctime(&end_time);
		out << login << endl;
		out << "Correct answers: " << percent << " %" << endl;
		out << "-------------------------------------" << endl;
		out.close();

		system("pause");
	}
	void fill_and_play(const string& answers, const string& pathToTest, const string& user)noexcept
	{
		for (size_t i = 0; i < answers.size(); i++)
		{
			correctness.push_back(answers[i] - 48);
		}
		this->play(pathToTest, user, answers.size());
	}
};