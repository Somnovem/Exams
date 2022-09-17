#pragma once
#include <iostream>
#include <string>
#include <list>
using namespace std;
enum Priority {LOW,MEDIUM,HIGH,EXTREME};
class Time
{
	size_t hours;
	size_t minutes;
	size_t seconds;
public:

};

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
};

class TaskList
{
public:
	string name;
	list<Task> tasks;
	TaskList(string n) : name{ n } {}
};

