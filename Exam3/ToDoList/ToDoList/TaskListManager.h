#pragma once
#include "Task.h"
#include <algorithm>

class TaskListManager
{
	static TaskListManager* instance;

	TaskListManager() {};

public:
	TaskList* taskList = nullptr;
	static TaskListManager* getinstance();
	void menu() { cout << "WE GOT THIS TASK LIST" << endl; };
	void setTaskList(TaskList* _Other) { taskList = _Other; }

};

TaskListManager* TaskListManager::instance = nullptr;

TaskListManager* TaskListManager::getinstance()
{
	if (instance == nullptr)
	{
		instance = new TaskListManager();
	}
	return instance;
}

