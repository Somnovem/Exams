#pragma once
enum PRIORITY
{
	LOW,MEDIUM,HIGH,SUPERHIGH
};
template<class T>
class Data
{
public:
	T value;
	PRIORITY pri = PRIORITY::LOW;
	Data* next = nullptr;
	Data* prev = nullptr;
};
