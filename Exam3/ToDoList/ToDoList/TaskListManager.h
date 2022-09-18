#pragma once
#include "Task.h"
#include <vector>
#include <algorithm>

class TaskListManager
{
	static TaskListManager* instance;
	std::shared_ptr<TaskList> taskList = nullptr;
	TaskListManager() {};
	void addTask();
	void deleteTask();
	void changeTask();
	void clear();
	void fullView();
public:
	static TaskListManager* getinstance();
	void menu();
	void setTaskList(std::shared_ptr<TaskList> _Other) { taskList = _Other; }
	bool searchConcrete(const string& taska);
	void searchTag(const string& taga);
	void searchTime(const Time& less);
	void searchPriority(const Priority& less);
	void searchDeadline();
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

void TaskListManager::addTask()
{
	//show what tasks are already present
	Task newTask;
	system("cls");
	//getting the task itself
	{
		string task;
		cout << "Task: ";
		getline(cin, task);
		newTask.setTask(task);
	}
	//getting priority
	newTask.setPriority(getPriority());
	//getting deadline
	{
		Time temp(getTime());
		newTask.setDeadline(temp);
	}
	//getting tag
	{
		system("cls");
		gotoxy(15, 9);
		cout << "Want to add tag?" << endl;
		bool c = Menu::select_vertical({"Yes","No"},HorizontalAlignment::Center,11);
		if (c)
		{
			newTask.setTag("");
		}
		else
		{
			string Tag;
			cout << "Tag: ";
			getline(cin, Tag);
			newTask.setTag(Tag);
		}

	}
	taskList->tasks.push_back(newTask);
}

void TaskListManager::deleteTask()
{
	system("cls");
	vector<string> tasks;
	for_each(taskList->tasks.begin(), taskList->tasks.end(), [&tasks](Task& task) {tasks.push_back(task.getTask()); });
	if (tasks.size() == 0)
	{
		cout << "No records" << endl;
		system("pause");
		return;
	}
	tasks.push_back("Exit");
	int c = Menu::select_vertical(tasks, HorizontalAlignment::Center, 11);
	if (c == tasks.size()-1)
	{
		return;
	}
	auto b = taskList->tasks.begin();
	advance(b, c);
	taskList->tasks.erase(b);
}

void TaskListManager::clear()
{
	system("cls");
	gotoxy(20, 10);
	cout << "Are you sure you want to clear the to-do list?" << endl;
	bool c = Menu::select_vertical({ "No","Yes" }, HorizontalAlignment::Center, 12);
	if (c)
	{
		taskList->tasks.clear();
	}
}

void TaskListManager::fullView()
{
	auto viewPriority = [](Priority prio)->string
	{
		switch (prio)
		{
		case LOW:
			return "Low";
			break;
		case MEDIUM:
			return "Medium";
			break;
		case HIGH:
			return "High";
			break;
		case EXTREME:
			return "EXTREME";
			break;
		default:
			break;
		}
	};
	system("cls");
	if (taskList->tasks.empty())
	{
		cout << "No records" << endl;
		system("pause");
		return;
	}
	for_each(taskList->tasks.begin(), taskList->tasks.end(), [&](Task& task)
		{
			cout << "Task: " << task.getTask() << endl;
			cout << "Priority: " << viewPriority(task.getPriority());
			cout << "Deadline: " << task.getDeadline() << endl;
			cout << "Tag: " << task.getTag() << endl;
			cout << "------------------------------" << endl;
		});
	system("pause");
}

void TaskListManager::changeTask()
{
	auto task = taskList->tasks.begin();
	//choosing task to change
	{
		vector<string> tasks;
		for_each(taskList->tasks.begin(), taskList->tasks.end(), [&](Task& t) {tasks.push_back(t.getTask()); });
		if (tasks.size() == 0)
		{
			cout << "No records" << endl;
			system("pause");
			return;
		}
		int choice = Menu::select_vertical(tasks, HorizontalAlignment::Center, 9);
		advance(task, choice);
	}
	auto changeQuest = [&task]()
	{
		string temp;
		cout << "New task: ";
		getline(cin, temp);
		(*task).setTask(temp);
	};
	auto changeDeadline = [&task]()
	{
		Time temp = getTime();
		(*task).setDeadline(temp);
	};
	auto changePriority = [&task]()
	{
		task->setPriority(getPriority());
	};
	auto changeTag = [&task]()
	{
		string temp;
		cout << "New tag: ";
		getline(cin, temp);
		(*task).setTag(temp);
	};
	int c = 0;
	while (c < 4)
	{
		system("cls");
		c = Menu::select_vertical({"Change task","Change deadline","Change priority","Change tag","Exit"}, HorizontalAlignment::Left, 15);
		switch (c)
		{
		case 0:
			changeQuest();
			break;
		case 1:
			changeDeadline();
			break;
		case 2:
			changePriority();
			break;
		case 3:
			changeTag();
			break;
		default:
			break;
		}
	}
}

void TaskListManager::menu()
{
	int c = 0;
	while (c < 5)
	{
		system("cls");
		{
			int m = 8;
			for_each(taskList->tasks.begin(), taskList->tasks.end(), [&m](Task& task)
				{
					if (m < 18)
					{
						gotoxy(35, m);
						cout << task.getTask() << " " << task.getPriority() << " " << task.getDeadline() << endl;
					}
					m++;
				});
			if (m > 18)
			{
				gotoxy(40, 20);
				cout << m - 18 << " more tasks in the list" << endl;
			}
		}
		c = Menu::select_vertical({"Add task","Delete task","Change task","Clear list","View full list","Exit"},HorizontalAlignment::Left,15);
		switch (c)
		{
		case 0:
			addTask();
			break;
		case 1:
			deleteTask();
			break;
		case 2:
			changeTask();
			break;
		case 3:
			clear();
			break;
		case 4:
			fullView();
			break;
		default:
			break;
		}
	}
}

bool TaskListManager::searchConcrete(const string& taska)
{
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getTask().find(taska) != string::npos)
		{
			cout <<"List: " << taskList->name << endl;
			b->print();
			return true;
		}
	}
	return false;
}

void TaskListManager::searchTag(const string& taga)
{
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getTag().find(taga) != string::npos)
		{
			cout << "List: " << taskList->name << endl;
			b->print();
		}
	}
}

void TaskListManager::searchTime(const Time& less)
{
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getDeadline() < less)
		{
			cout << "List: " << taskList->name << endl;
			b->print();
		}
	}
}

void TaskListManager::searchPriority(const Priority& less)
{
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getPriority() < less)
		{
			cout << "List: " << taskList->name << endl;
			b->print();
		}
	}
}

void TaskListManager::searchDeadline()
{
	Time current;
	current.setCurrent();
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (!(b->getDeadline() < current) || b->withinDay(current))
		{
			cout << "List: " << taskList->name << endl;
			b->print();
		}
	}
}