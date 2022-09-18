#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <chrono>
#include <ctime>
#include <experimental/filesystem>
#include "Menu.h"
using namespace std;
namespace fs = std::experimental::filesystem;

enum Priority {LOW,MEDIUM,HIGH,EXTREME};
enum Month {JAN = 1,FBR,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC};


ostream& operator<<(ostream& out, const Priority& prio)
{
	switch (prio)
	{
	case LOW:
		out << "Low";
		break;
	case MEDIUM:
		out << "Medium";
		break;
	case HIGH:
		out << "High";
		break;
	case EXTREME:
		out << "Extreme";
		break;
	default:
		break;
	}
	return out;
}
ostream& operator<<(ostream& out,const Month& mnt)
{
	switch (mnt)
	{
	case JAN:
		out << "JAN";
		break;
	case FBR:
		out << "FBR";
		break;
	case MAR:
		out << "MAR";
		break;
	case APR:
		out << "APR";
		break;
	case MAY:
		out << "MAY";
		break;
	case JUN:
		out << "JUN";
		break;
	case JUL:
		out << "JUL";
		break;
	case AUG:
		out << "AUG";
		break;
	case SEP:
		out << "SEP";
		break;
	case OCT:
		out << "OCT";
		break;
	case NOV:
		out << "NOV";
		break;
	case DEC:
		out << "DEC";
		break;
	default:
		break;
	}
	return out;
}
ofstream& operator<<(ofstream& out, const Priority& prio)
{
	out << static_cast<int>(prio);
	return out;
}
ofstream& operator<<(ofstream& out, const Month& mnt)
{
	switch (mnt)
	{
	case JAN:
		out << "JAN";
		break;
	case FBR:
		out << "FBR";
		break;
	case MAR:
		out << "MAR";
		break;
	case APR:
		out << "APR";
		break;
	case MAY:
		out << "MAY";
		break;
	case JUN:
		out << "JUN";
		break;
	case JUL:
		out << "JUL";
		break;
	case AUG:
		out << "AUG";
		break;
	case SEP:
		out << "SEP";
		break;
	case OCT:
		out << "OCT";
		break;
	case NOV:
		out << "NOV";
		break;
	case DEC:
		out << "DEC";
		break;
	default:
		break;
	}
	return out;
}


Month stoM(const string& str)
{
	if (str == "Sep") return SEP;
	else if (str == "Jan") return JAN;
	else if (str == "Fbr") return FBR;
	else if (str == "Mar") return MAR;
	else if (str == "Apr") return APR;
	else if (str == "May") return MAY;
	else if (str == "Jun") return JUN;
	else if (str == "Jul") return JUL;
	else if (str == "Aug") return AUG;
	else if (str == "Oct") return OCT;
	else if (str == "Nov") return NOV;
	else return DEC;
}
class Time
{
public:
	size_t hours;
	size_t minutes;
	size_t seconds;
	size_t day;
	Month month;
	Time(size_t h = 0, size_t m = 0, size_t s = 0, size_t d = 1, Month mon = JAN) : hours{ h }, minutes{ m }, seconds{ s }, day{ d }, month{mon}{}
	bool operator< (const Time& _Other)
	{
		if (this->month < _Other.month || this->day < _Other.day || this->hours < _Other.hours || this->minutes  < _Other.minutes || this->seconds < _Other.seconds)
		{
			return true;
		}
		return false;
	}
	friend ostream& operator<<(ostream& out,const Time& t);
	Time(const Time& _Other)
	{
		this->hours = _Other.hours;
		this->minutes = _Other.minutes;
		this->seconds = _Other.seconds;
		this->day = _Other.day;
		this->month = _Other.month;
	}
	void setCurrent()
	{
		auto a = std::chrono::system_clock::now();
		time_t temp = std::chrono::system_clock::to_time_t(a);
		string time = ctime(&temp);
		month = stoM(time.substr(4, 3));
		day = stoi(time.substr(8,2));
		hours = stoi(time.substr(11, 2));
		minutes = stoi(time.substr(14, 2));
		seconds = stoi(time.substr(17, 2));
	}
	Time& operator=(const Time& _Other) 
	{
		if (&_Other == this) return *this;

		this->hours = _Other.hours;
		this->minutes = _Other.minutes;
		this->seconds = _Other.seconds;
		this->day = _Other.day;
		this->month = _Other.month;
	}
};

Time getTime()
{
	system("cls");
	auto getWithCheck = [](size_t& param,size_t higher)->void
	{
		int buff;
		cin >> buff;
		if (buff < 0) buff = 0;
		else if (buff >= higher) buff = higher-1;
		param = buff;
	};
		size_t h, m, s,d,mon;
		cout << "Hours: ";
		getWithCheck(h,24);
		cout << "Minutes: ";
		getWithCheck(m, 60);
		cout << "Seconds: ";
		getWithCheck(s, 60);
		cout << "Day: ";
		getWithCheck(d, 32);
		cout << "Month: ";
		getWithCheck(mon, 13);
		cin.ignore();
		return Time(h,m,s,d,Month(mon));
}
ostream& operator<<(ostream& out, const Time& t)
{
	out << t.hours << ":" << t.minutes << ":" << t.seconds << " " << t.day<< " " << t.month;
	return out;
}

class Task
{
	string task;
	Priority priority;
	Time deadline;
	string tag;
public:
	Task() {}
	string getTask() { return task; }
	void setTask(const string& _Other) { task = _Other; }
	Priority getPriority() { return priority; }
	void setPriority(const Priority& _Other) { priority = _Other; }
	Time getDeadline() { return deadline; }
	void setDeadline(const Time& _Other) { deadline = _Other; }
	string getTag() { return tag; }
	void setTag(const string& _Other) { tag = _Other; }
	void print()
	{
		cout << "Task: " << task << endl;
		cout << "Deadline: " << deadline << endl;
		cout << "Priority: " << priority << endl;
		cout << "Tag: " <<tag << endl;
		cout << "--------------------------------------------" << endl;
	}
	bool withinDay(const Time& _Other)
	{
		Time temp = deadline;
		temp.day += 1;
		return !(temp < _Other);
	}
};
Priority getPriority()
{
	system("cls");
	gotoxy(20, 10);
	cout << "Choose priority: ";
	int c = Menu::select_vertical({ "Low","Medium","High","Extreme" }, HorizontalAlignment::Center, 11);
	return (Priority(c));
}

class TaskList
{
public:
	string name;
	list<Task> tasks;
	TaskList(string n) : name{ n } {}
	void save()
	{
		ofstream out("To-Do\\" + name + ".txt");
		for_each(tasks.begin(), tasks.end(), [&out](Task& task) 
	    {
				out << task.getTask() << endl;
				out << task.getDeadline() << endl;
				out << task.getPriority() << endl;
				out << task.getTag() << endl;
	    });
		out.close();
	}
	void load() {}
};

