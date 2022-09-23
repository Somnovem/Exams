#pragma once
#include "SearchCommand.h"

TaskListFacade* TaskListFacade::instance = nullptr;

TaskListFacade* TaskListFacade::getinstance()
{
	if (instance == nullptr)
	{
		instance = new TaskListFacade();
	}
	return instance;
}

void TaskListFacade::addTask()
{
	//show what tasks are already present
	Task newTask;
	system("cls");
	{
		int m = 2;
		gotoxy(30, m++);
		cout << "Already exist: " << endl;
		for_each(taskList->tasks.begin(), taskList->tasks.end(), [&](Task& task)
			{
				gotoxy(30, m++);
				cout << task.getTask() << endl;
			});
	}
	//getting the task itself
	{
		string task;
		gotoxy(2, 15);
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
		if (getNoOrYes("Want to add tag?"))
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
	taskList->sort();
}

void TaskListFacade::deleteTask()
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

void TaskListFacade::clear()
{
	if (getNoOrYes("Are you sure you want to clear the list?"))
	{
		taskList->tasks.clear();
	}
}

void TaskListFacade::fullView()const
{
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
			cout << "Priority: " << task.getPriority() << endl;
			cout << "Deadline: " << task.getDeadline() << endl;
			if (!task.getTag().empty())
			{
				cout << "Tag: " << task.getTag() << endl;
			}
			cout << "------------------------------" << endl;
		});
	system("pause");
}

void TaskListFacade::changeTask()
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

void TaskListFacade::menu()
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

bool TaskListFacade::searchConcrete(const string& task) const
{
	system("cls");
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getTask().find(task) != string::npos)
		{
			cout <<"List: " << taskList->name << endl;
			b->print();
			system("pause");
			return true;
		}
	}
	return false;
}

bool TaskListFacade::searchTag(const string& tag) const
{
	bool found = false;
	system("cls");
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getTag().find(tag) != string::npos)
		{
			cout << "List: " << taskList->name << endl;
			b->print();
			found = true;
		}
	}
	if (found)
	{
		system("pause");
	}

	return found;
}

bool TaskListFacade::searchTime(const Time& less) const
{
	bool found = false;
	system("cls");
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getDeadline() < less)
		{
			cout << "List: " << taskList->name << endl;
			b->print();
			found = true;
		}
	}
	if (found)
	{
		system("pause");
	}
	return found;
}

bool TaskListFacade::searchPriority(const Priority& less) const
{
	bool found = false;
	system("cls");
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getPriority() == less)
		{
			cout << "List: " << taskList->name << endl;
			b->print();
			found = true;
		}
	}
	if (found)
	{
		system("pause");
	}
	return found;
}

bool TaskListFacade::searchDeadline() const
{
	bool found = false;
	system("cls");
	Time current;
	current.setCurrent();
	for (size_t i = 0; i < taskList->tasks.size(); i++)
	{
		auto b = taskList->tasks.begin();
		advance(b, i);
		if (b->getDeadline() < current || b->withinDay(current))
		{
			cout << "List: " << taskList->name << endl;
			b->print();
			found = true;
		}
	}
	if (found)
	{
		system("pause");
	}
	return found;
}

void TaskListFacade::save() const
{
		taskList->save();
}

void TaskListFacade::load()
{
	ifstream in("To-Do\\" + taskList->name + ".txt");
	string newTask, newDeadline, newPriority, newTag;
	while (getline(in,newTask) && getline(in,newDeadline) && getline(in,newPriority) && getline(in,newTag))
	{
		Task temp;
		temp.load(newTask, newDeadline, newPriority, newTag);
		taskList->tasks.push_back(temp);
	}
	taskList->sort();
}