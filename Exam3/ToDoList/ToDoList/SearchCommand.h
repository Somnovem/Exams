#pragma once
#include "Task.h"
#include <vector>
#include <algorithm>
using namespace std;

class TaskListFacade
{
	static TaskListFacade* instance;
	std::shared_ptr<TaskList> taskList = nullptr;
	TaskListFacade() {};
	void addTask();
	void deleteTask();
	void changeTask();
	void clear();
	void fullView() const;
public:
	static TaskListFacade* getinstance();
	void menu();
	void setTaskList(std::shared_ptr<TaskList> _Other) { taskList = _Other; }
	bool searchConcrete(const string& taska) const;
	bool searchTag(const string& taga) const;
	bool searchTime(const Time& less) const;
	bool searchPriority(const Priority& less) const;
	bool searchDeadline() const;
	void save() const;
	void load();
};

class ISearchCommand
{
protected:
	bool isFound = false;
	list<std::shared_ptr<TaskList>>* List;
	TaskListFacade* manager;
public:
	ISearchCommand(list<std::shared_ptr<TaskList>>* l, TaskListFacade* m) : List{ l }, manager{ m } {}
	virtual ~ISearchCommand() { }
	virtual void execute() = 0;
	void checkMatches(bool logic)
	{
		if (!logic)
		{
			cout << "No matches :(" << endl;
			system("pause");
		}
	}
};

class SearchConcrete : public ISearchCommand
{
public:
	SearchConcrete(list<std::shared_ptr<TaskList>>* l, TaskListFacade* m) : ISearchCommand(l,m) {}
	virtual void execute() override
	{
		system("cls");
		gotoxy(20, 14);
		cout << "Task to search for: ";
		string temp;
		getline(cin, temp);
		for_each(List->begin(), List->end(), [&](std::shared_ptr<TaskList> ptr)
			{
				if (!isFound)
				{
					manager->setTaskList(ptr);
					isFound = manager->searchConcrete(temp);
				}

			});
		checkMatches(isFound);

	}
};

class SearchTag : public ISearchCommand
{
public:
	SearchTag(list<std::shared_ptr<TaskList>>* l, TaskListFacade* m) : ISearchCommand(l, m) {}
	virtual void execute() override
	{
		string temp;
		system("cls");
		gotoxy(20, 14);
		cout << "Tag to search for: ";
		getline(cin, temp);
		for_each(List->begin(), List->end(), [&](std::shared_ptr<TaskList> ptr)
			{
				manager->setTaskList(ptr);
				isFound = manager->searchTag(temp);
			});
		checkMatches(isFound);
		system("pause");
	}
};

class SearchDeadline : public ISearchCommand
{
public:
	SearchDeadline(list<std::shared_ptr<TaskList>>* l, TaskListFacade* m) : ISearchCommand(l, m) {}
	virtual void execute() override
	{
		for_each(List->begin(), List->end(), [&](std::shared_ptr<TaskList> ptr)
			{
				manager->setTaskList(ptr);
				isFound = manager->searchDeadline();
			});
		checkMatches(isFound);
		system("pause");
	}
};

class SearchDate : public ISearchCommand
{
public:
	SearchDate(list<std::shared_ptr<TaskList>>* l, TaskListFacade* m) : ISearchCommand(l, m) {}
	virtual void execute() override
	{
		system("cls");
		gotoxy(20, 14);
		cout << "Now you will input the deadline till which to search for" << endl;
		system("pause");
		Time less = getTime();
		for_each(List->begin(), List->end(), [&](std::shared_ptr<TaskList> ptr)
			{
				manager->setTaskList(ptr);
				isFound = manager->searchTime(less);
			});
		checkMatches(isFound);
		system("pause");
	}
};

class SearchPriority : public ISearchCommand
{
public:
	SearchPriority(list<std::shared_ptr<TaskList>>* l, TaskListFacade* m) : ISearchCommand(l, m) {}
	virtual void execute() override
	{
		Priority less = getPriority();
		for_each(List->begin(), List->end(), [&](std::shared_ptr<TaskList> ptr)
			{
				manager->setTaskList(ptr);
				isFound = manager->searchPriority(less);
			});
		checkMatches(isFound);
		system("pause");
	}
};