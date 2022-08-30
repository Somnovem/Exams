#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
using namespace std;
class MyException abstract
{
	string err;
public:
	MyException(string e, string date, std::chrono::system_clock::time_point time, string file, int line)
	{
		time_t tt = std::chrono::system_clock::to_time_t(time);
		std::tm tm = *std::localtime(&tt);
		stringstream ss;
		string format = "UTC: %Y-%m-%d %H:%M:%S";
		ss << std::put_time(&tm, format.c_str());
		err = e + date + ss.str() + file + to_string(line);
		saveLog();
	}
	string what() { return err; }
	void saveLog()
	{
		ofstream out("log.txt", ios::app);
		out << err << endl;
		out.close();
	}
};

class __OUT_OF_MEMORY__ : public MyException
{
public:
	__OUT_OF_MEMORY__() : MyException("Out of memory ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) { }
};

class __INVALID_INDEX__ : public MyException
{
public:
	__INVALID_INDEX__() : MyException("Invalid index ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) {}
};

class __DIVISION_BY_ZERO__ : public MyException
{
public:
	__DIVISION_BY_ZERO__() : MyException("Division by zero ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) { }
};

class __DELETING_FROM_AN_EMPTY_CONTAINER__ : public MyException
{
public:
	__DELETING_FROM_AN_EMPTY_CONTAINER__() : MyException("Deleting from an empty container ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) { }
};

class __READING_FROM_AN_EMPTY_CONTAINER__ : public MyException
{
public:
	__READING_FROM_AN_EMPTY_CONTAINER__() : MyException("Reading from an empty container ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) { }
};

class __ROOT_FROM_NEGATIVE__ : public MyException
{
public:
	__ROOT_FROM_NEGATIVE__() : MyException("Root from negative ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) { }
};

class __READING_FILE_FAILED : public MyException
{
public:
	__READING_FILE_FAILED() : MyException("Failed to read a file ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) { }
};

class __WRITING_TO_FILE_FAILED__ : public MyException
{
public:
	__WRITING_TO_FILE_FAILED__() : MyException("Failed to write to file ", __DATE__, std::chrono::system_clock::now(), __FILE__, __LINE__) { }
};

