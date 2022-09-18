#pragma once
#include "TaskListManager.h"
int chooseList(list<std::shared_ptr<TaskList>> List)
{
	vector<string> lists;
	for_each(List.begin(), List.end(), [&lists](std::shared_ptr<TaskList> tl)
		{
			lists.push_back((*tl).name);
		});
	if (lists.size() == 0)
	{
		cout << "No records" << endl;
		system("pause");
		return -1;
	}
	lists.push_back("Exit");
	int c = Menu::select_vertical(lists, HorizontalAlignment::Center, 9);
	if (c == lists.size() - 1)
	{
		return -1;
	}
	return c;
}
class ToDoManager
{
	static ToDoManager* instance;
	list<std::shared_ptr<TaskList>> List;
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
		int c = 0;
		while (c < 4)
		{
			system("cls");
			c = Menu::select_vertical({"Add list","Delete list","Configure list","Search","Exit"}, HorizontalAlignment::Left, 15);
			switch (c)
			{
			case 0:
				addList();
				break;
			case 1:
				deleteList();
				break;
			case 2:
				configureList();
				break;
			case 3:
				search();
				break;
			default:
				break;
			}
		}
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
	List.push_back(make_shared<TaskList>(bs));
}

void ToDoManager::deleteList()
{
	int c = chooseList(List);
	if (c < 0)
	{
		return;
	}
	auto b = List.begin();
	advance(b, c);
	List.erase(b);
}

void ToDoManager::configureList()
{


	int c = chooseList(List);
	if (c < 0)
	{
		return;
	}
	auto list = List.begin();
	advance(list, c);
	manager->setTaskList(*list);
	manager->menu();
}

void ToDoManager::search()
{
	auto searchConcrete = [&]()
	{
		bool isFound = false;
		system("cls");
		gotoxy(20, 14);
		cout << "Task to search for: ";
		string temp;
		getline(cin, temp);
		for_each(List.begin(), List.end(), [&](std::shared_ptr<TaskList> ptr)
		{	
				if (!isFound)
				{
					manager->setTaskList(ptr);
					isFound = manager->searchConcrete(temp);
				}
				
		});
		if (!isFound)
		{
			cout << "No matches :(" << endl;
			system("pause");
		}
	};
	auto searchTag = [&]()
	{
		string temp;
		system("cls");
		gotoxy(20, 14);
		cout << "Tag to search for: ";
		getline(cin, temp);
		for_each(List.begin(), List.end(), [&](std::shared_ptr<TaskList> ptr)
		{
					manager->setTaskList(ptr);
					manager->searchTag(temp);
		});

	};
	auto searchDate = [&]()
	{
		system("cls");
		gotoxy(20, 14);
		cout << "Now you will input the deadline till which to search for" << endl;
		system("pause");
		Time less = getTime();
		for_each(List.begin(), List.end(), [&](std::shared_ptr<TaskList> ptr)
			{
				manager->setTaskList(ptr);
				manager->searchTime(less);
			});
	};
	auto searchPriority = [&]()
	{
		Priority less = getPriority();
		for_each(List.begin(), List.end(), [&](std::shared_ptr<TaskList> ptr)
		{
				manager->setTaskList(ptr);
				manager->searchPriority(less);
		});
	};
	auto searchDeadline = [&]()
	{
		for_each(List.begin(), List.end(), [&](std::shared_ptr<TaskList> ptr)
			{
				manager->setTaskList(ptr);
				manager->searchDeadline();
			});
	};
	int c = 0;
	while (c < 5)
	{
		system("cls");
		c = Menu::select_vertical({"Search concrete task","Search by tag","Search by date","Search by priority","Show all out of deadline or within 1 day","Exit"}, HorizontalAlignment::Left, 15);
		switch (c)
		{
		case 0:
			searchConcrete();
			break;
		case 1:
			searchTag();
			break;
		case 2:
			searchDate();
			break;
		case 3:
			searchPriority();
			break;
		case 4:
			searchDeadline();
			break;
		default:
			break;
		}
	}
}
