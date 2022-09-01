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
	Question() {}
	Question(const Question& _Other)
	{
		this->question = _Other.question;
		this->answers = _Other.answers;
		this->correct = _Other.correct;
	}
	Question& operator=(const Question& _Other)
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
	bool pass()
	{
		system("cls");
		gotoxy(20, 6);
		cout << question << endl;
		int c = Menu::select_vertical(answers, HorizontalAlignment::Center, 7);
		if (c == correct)
		{
			return true;
		}
		return false;
	}
};
class Test
{
	vector<Question> test;
public:

	void constructTest(string path)
	{
		vector<string> quests;
		for (const auto& file : fs::directory_iterator(path))
		{
			quests.push_back(file.path().u8string());
			cout << file.path().u8string() << endl;
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
	void addQuestion(const Question& quest)
	{
		test.push_back(quest);
	}
	void play(const string& pathToTest,const string& user)
	{
		
		auto start = std::chrono::system_clock::now();
		vector<bool> correctness;
		for_each(test.begin(), test.end(), [&correctness](Question temp) {correctness.push_back(temp.pass()); });
		auto end = std::chrono::system_clock::now();
		system("cls");
		size_t correct = count(correctness.begin(), correctness.end(), true);
		double percent = (static_cast<double>(correct) / test.size()) * 100;
		int m = 13;
		gotoxy(20, m++);
		cout << "Correct amswers: " << correct << " / " << test.size() << endl;
		gotoxy(20, m++);
		cout << "Percentage:  " << percent << "%" << endl;
		cout.precision(2);
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		if (correct != test.size())
		{
			auto findMistakes = [&correctness]()
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
		string login;
		login.assign(user.begin() + user.find_last_of("\\") + 1, user.begin() + user.find_last_of("."));
		ofstream out("Data\\Statistics\\" + testName  + ".txt",ios::app);
		out << "Time spent: " << elapsed_seconds.count() << " s" << endl;
		out << "Finished at: " <<  std::ctime(&end_time);
		out << login << endl;
		out << "Correct amswers: " << correct << " / " << test.size() << endl;
		out << "-------------------------------------" << endl;
		out.close();
		system("pause");
	}
};