#pragma once
#include <iostream>
using namespace std;
class User abstract
{
protected:
	std::string name;
	int age;
	std::string phone;
	std::string adress;
    public:
	virtual void menu() = 0;
};

class Guest : public User
{
	virtual void menu() noexcept override {}
};

class Admin : public User
{
	virtual void menu()noexcept override {}
};

