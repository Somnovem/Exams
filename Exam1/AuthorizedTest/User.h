#pragma once
class Aplication
{
	User* user = nullptr;
	void reg() {}
	void log() {}
	void exit() {}
};
class User abstract
{protected:
	string name;
	int age;
	string phone;
	string adress;
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

