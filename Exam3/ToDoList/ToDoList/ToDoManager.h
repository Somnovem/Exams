#pragma once
#include "TaskListManager.h"

class ToDoManager
{
	static ToDoManager* instance;
	list<TaskList*> List;
	TaskListManager* manager = TaskListManager::getinstance();
	ToDoManager() {};
	void addList();
	void deleteList();
	void configureList();
	void search();
public:
	static ToDoManager* getinstance();
	void menu()
	{
		deleteList();
	};
};

ToDoManager* ToDoManager::instance = nullptr;

ToDoManager* ToDoManager::getinstance()
{
	if (instance == nullptr)
	{
		instance = new ToDoManager();
	}	
	return instance;
}

void ToDoManager::addList()
{
	string temp;
	cout << "Name of the new list: ";
	getline(cin, temp);
	TaskList bs(temp);
	List.push_back(&bs);
}

void ToDoManager::deleteList()
{

}
