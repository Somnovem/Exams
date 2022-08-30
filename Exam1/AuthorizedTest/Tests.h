#pragma once
#include <iostream>
#include <vector>
#include "../Library/Menu.h"
#include "../Library/Functions.h"
#include <algorithm>
#include <fstream>
#include <string>
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
		if (this)
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
			cout << "Correct" << endl;
			return true;
		}
		return 0;
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
	void play()
	{
		int correct = 0;
		for_each(test.begin(), test.end(), [&correct](Question temp) {correct+=temp.pass(); });
		system("cls");
		double percent = (correct / test.size())*100;
		gotoxy(20, 13);
		cout << "Correct amswers: " << correct << " / " << test.size() << endl;
		gotoxy(20, 14);
		cout << "Percentage:  " << percent << "%" << endl;
		system("pause");
	}
};