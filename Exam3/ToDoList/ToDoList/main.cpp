#include "ToDoManager.h"

int main()
{
	ToDoFacade* application = ToDoFacade::getinstance();
	application->menu();
}