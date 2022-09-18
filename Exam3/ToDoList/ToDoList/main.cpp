#include "ToDoManager.h"

int main()
{

	ToDoManager* application = ToDoManager::getinstance();
	application->menu();
}