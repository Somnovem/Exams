#pragma once
#include"../Library/Data.h"
#include "../Library/Functions.h"
#include "../Library/MyException.h"
#include<initializer_list>
	template<class T>
	class List
	{
		Data<T>* first = nullptr;
		Data<T>* last = nullptr;
		size_t   size = 0;
		void copy(const List<T>& list) noexcept;
	public:
		List() {}
		List(initializer_list<T> list) noexcept;
		~List() noexcept;
		List(const List<T>& list) noexcept;
		List<T>& operator=(const List<T>& list) noexcept;

		void push_back(const T& value);
		void push_front(const T& value);
		void insert(const T& value, size_t ind);

		void pop_back();
		void pop_front();
		void remove(size_t ind);

		T& front() const;
		T& back() const;
		T& at(size_t ind) const;

		T& operator[](size_t ind) const;
		void resize(size_t newSize) noexcept;
		void swap(List<T>& Right) noexcept;

		List<T> operator+(const List<T>& list) const noexcept;
		void operator+=(const List<T>& list) noexcept;
		List<T> operator*(const List<T>& list) const noexcept;
		List<T> operator/(const List<T>& list) const noexcept;

		bool operator==(const List<T>& list) const noexcept;
		bool operator!=(const List<T>& list) const noexcept;

		bool isEmpty() const noexcept;
		size_t length() const noexcept;
		void clear() noexcept;
		void print() const noexcept;
		void rprint() const noexcept;
		void sort(bool(*method)(T a, T b) = asc) noexcept;
		void reverse() noexcept;
		void splice(List<T>& other, size_t ind, size_t count) noexcept;
		size_t find(T value) const noexcept;
	};

	template<class T>
	void List<T>::copy(const List<T>& list) noexcept
	{
		this->clear();
		Data<T>* pos = list.first;
		while (pos)
		{
			push_back(pos->value);
			pos = pos->next;
		}
	}

	template<class T>
	List<T>::List(const List<T>& list) noexcept
	{
		this->copy(list);
	}

	template<class T>
	List<T>::List(initializer_list<T> list)noexcept
	{
		for (T l : list)
		{
			this->push_back(l);
		}
	}

	template<class T>
	List<T>& List<T>::operator=(const List<T>& list) noexcept
	{
		List<T> temp(list);
		return temp;
	}

	template<class T>
	List<T>::~List() noexcept
	{
		this->clear();
	}

	template<class T>
	void List<T>::push_back(const T& value)
	{
		if (size == 0)
		{
			first = new Data<T>;

			try
			{
				if (!first)
				{
					throw __OUT_OF_MEMORY__();
				}
				first->value = value;
				last = first;
			}
			catch (...)
			{
				return;
			}
		}
		else
		{
			last->next = new Data<T>;
			try
			{
				if (!last->next)
				{
					throw __OUT_OF_MEMORY__();
				}
			}
			catch (...)
			{
				return;
			}
			last->next->value = value;
			last->next->prev = last;
			last = last->next;
		}
		size++;
	}

	template<class T>
	void List<T>::push_front(const T& value)
	{
		if (size == 0)
		{
			first = new Data<T>;
			try
			{
				if (!first)
				{
					throw __OUT_OF_MEMORY__();
				}
			}
			catch (...)
			{
				return;
			}

			first->value = value;
			last = first;
		}
		else
		{
			first->prev = new Data<T>;
			try
			{
				if (!first->prev)
				{
					throw __OUT_OF_MEMORY__();
				}
			}
			catch (...)
			{
				return;
			}

			first->prev->value = value;
			first->prev->next = first;
			first = first->prev;
		}
		size++;
	}

	template<class T>
	void List<T>::insert(const T& value, size_t ind)
	{
		try
		{
			if (ind < 0 || ind > size)
			{
				throw __INVALID_INDEX__();
			}
		}
		catch (...)
		{
			return;
		}

		if (ind == 0)
		{
			this->push_front(value);
		}
		else if (ind == size)
		{
			this->push_back(value);
		}
		else
		{
			Data<T>* pos;
			if (ind <= size / 2)
			{
				pos = first;
				for (size_t i = 0; i < ind - 1; i++)
				{
					pos = pos->next;
				}
			}
			else
			{
				pos = last;
				for (size_t i = 0; i < size - ind; i++)
				{
					pos = pos->prev;
				}
			}
			Data<T>* temp = new Data<T>;
			try
			{
				if (!temp)
				{
					throw __OUT_OF_MEMORY__();
				}
			}
			catch (...)
			{
				return;
			}

			temp->value = value;
			temp->next = pos->next;
			temp->next->prev = temp;
			temp->prev = pos;
			pos->next = temp;
			size++;
		}

	}

	template<class T>
	void List<T>::pop_back()
	{
		try
		{
			if (size <= 0)
			{
				throw __DELETING_FROM_AN_EMPTY_CONTAINER__();
			}
		}
		catch (...)
		{
			return;
		}

		if (size == 1)
		{
			delete first;
			first = last = nullptr;
		}
		else
		{
			last = last->prev;
			delete last->next;
			last->next = nullptr;
		}
		size--;
	}

	template<class T>
	void List<T>::pop_front()
	{
		try
		{
			if (size <= 0)
			{
				throw __DELETING_FROM_AN_EMPTY_CONTAINER__();
			}
		}
		catch (...)
		{
			return;
		}

		if (size == 1)
		{
			delete first;
			first = last = nullptr;
		}
		else
		{
			first = first->next;
			delete first->prev;
			first->prev = nullptr;
		}
		size--;
	}

	template<class T>
	void List<T>::remove(size_t ind)
	{
		try
		{
			if (ind < 0 || ind > size)
			{
				throw __INVALID_INDEX__();
			}
		}
		catch (...)
		{
			return;
		}

		if (ind == 0)
		{
			this->pop_front();
		}
		else if (ind == size - 1)
		{
			this->pop_back();
		}
		else
		{
			Data<T>* pos;
			if (ind <= size / 2)
			{
				pos = first;
				for (size_t i = 0; i < ind; i++)
				{
					pos = pos->next;
				}
			}
			else
			{
				pos = last;
				for (size_t i = 0; i < size - ind - 1; i++)
				{
					pos = pos->prev;
				}
			}
			pos->prev->next = pos->next;
			pos->next->prev = pos->prev;
			delete pos;
			size--;
		}
	}

	template<class T>
	T& List<T>::front() const
	{
		try
		{
			if (size <= 0)
			{
				throw __READING_FROM_AN_EMPTY_CONTAINER__();
			}
		}
		catch (...)
		{
			cout << "Reading from an empty container" << endl;
			exit(-1);
		}

		return first->value;
	}

	template<class T>
	T& List<T>::back() const
	{
		try
		{
			if (size <= 0)
			{
				throw __READING_FROM_AN_EMPTY_CONTAINER__();
			}
		}
		catch (...)
		{
			cout << "Reading from an empty container" << endl;
			exit(-1);
		}

		return last->value;
	}

	template<class T>
	T& List<T>::at(size_t ind) const
	{
		try
		{
			if (ind < 0 || ind > size)
			{
				throw __INVALID_INDEX__();
			}
		}
		catch (...)
		{
			cout << "Invalid index " << endl;
			exit(-1);
		}
		Data<T>* pos = nullptr;
		if (ind < size / 2)
		{
			pos = first;
			for (size_t i = 0; i < ind; i++)
			{
				pos = pos->next;
			}
		}
		else
		{
			pos = last;
			for (size_t i = 0; i < size - ind - 1; i++)
			{
				pos = pos->prev;
			}
		}
		return pos->value;

	}

	template<class T>
	T& List<T>::operator[](size_t ind) const
	{
		try
		{
			if (ind < 0 || ind > size)
			{
				throw __INVALID_INDEX__();
			}
		}
		catch (...)
		{
			cout << "Invalid index " << endl;
			exit(-1);
		}

		return this->at(ind);
	}

	template<class T>
	void List<T>::resize(size_t newSize) noexcept
	{
		if (this->size == newSize)
		{
			return;
		}
		if (newSize < this->size)
		{
			for (size_t i = 0; i < this->size - newSize + 1; i++)
			{
				this->pop_back();
			}
			return;
		}
		T value{};
		for (size_t i = 0; i < newSize - this->size; i++)
		{
			this->push_back(value);
		}
	}

	template<class T>
	void List<T>::swap(List<T>& Right) noexcept
	{
		List<T> temp(*this);
		this->copy(Right);
		Right.copy(temp);
	}

	template<class T>
	bool List<T>::isEmpty() const noexcept
	{
		return size == 0;
	}

	template<class T>
	size_t List<T>::length() const noexcept
	{
		return size;
	}

	template<class T>
	void List<T>::clear() noexcept
	{
		while (first)
		{
			first = first->next;
			if (first)
			{
				delete first->prev;
			}
		}
		delete last;
		last = nullptr;
		size = 0;
	}

	template<class T>
	void List<T>::print() const noexcept
	{
		Data<T>* pos = first;
		while (pos)
		{
			cout << pos->value << " ";
			pos = pos->next;
		}
		cout << endl;
	}

	template<class T>
	void List<T>::rprint() const noexcept
	{
		Data<T>* pos = last;
		while (pos)
		{
			cout << pos->value << " ";
			pos = pos->prev;
		}
		cout << endl;
	}

	template<class T>
	void List<T>::sort(bool(*method)(T a, T b)) noexcept
	{
		for (size_t i = 0; i < size - 1; i++)
		{
			for (size_t j = 0; j < size - 1 - i; j++)
			{
				if (method(this->operator[](j), this->operator[](j + 1)))
				{
					swap(this->operator[](j), this->operator[](j + 1));
				}
			}
		}
	}

	template<class T>
	bool List<T>::operator==(const List<T>& list) const noexcept
	{
		if (this->size != list.size)
		{
			return 0;
		}
		Data<T>* pos = this->first;
		Data<T>* secondPos = list.first;
		while (pos)
		{
			if (pos->value != secondPos->value)
			{
				return 0;
			}
			pos = pos->next;
			secondPos = secondPos->next;
		}
		return 1;
	}

	template<class T>
	bool List<T>::operator!=(const List<T>& list) const noexcept
	{
		if (this->size != list.size)
		{
			return 1;
		}
		Data<T>* pos = this->first;
		Data<T>* secondPos = list.first;
		while (pos)
		{
			if (pos->value == secondPos->value)
			{
				return 0;
			}
			pos = pos->next;
			secondPos = secondPos->next;
		}
		return 1;
	}

	template<class T>
	List<T> List<T>::operator+(const List<T>& list) const noexcept
	{
		List<T> temp(*this);
		Data<T>* pos = list.first;
		while (pos)
		{
			temp.push_back(pos->value);
			pos = pos->next;
		}
		return temp;
	}

	template<class T>
	void List<T>::operator+=(const List<T>& list) noexcept
	{
		Data<T>* pos = list.first;
		while (pos)
		{
			this->push_back(pos->value);
			pos = pos->next;
		}
	}

	template<class T>
	void List<T>::reverse() noexcept
	{
		Data<T>* pos = first;
		Data<T>* secondPos = last;
		for (size_t i = 0; i < size / 2; i++)
		{
			swap(pos->value, secondPos->value);
			pos = pos->next;
			secondPos = secondPos->prev;
		}
	}

	template<class T>
	void List<T>::splice(List<T>& other, size_t ind, size_t count) noexcept
	{
		if (ind >= 0 && ind < other.size && ind + count <= other.size)
		{
			this->clear();
			Data<T>* lb = other.first;
			Data<T>* rb = other.first;
			for (size_t i = 0; i < ind; i++)
			{
				lb = lb->next;
			}
			for (size_t i = 0; i < ind + count - 1; i++)
			{
				rb = rb->next;
			}
			if (ind == 0)
			{
				other.first = rb->next;
			}
			else
			{
				lb->prev->next = rb->next;
			}
			if (ind + count == other.size)
			{
				other.last = lb->prev;
			}
			else
			{
				rb->next->prev = lb->prev;
			}
			other.size -= count;
			this->first = lb;
			this->last = rb;
			this->size = count;
			rb->next = nullptr;
		}
	}

	template<class T>
	size_t List<T>::find(T value) const noexcept
	{
		Data<T>* temp = this->first;
		size_t ind = 0;
		while (temp)
		{
			if (temp->value == value)
			{
				return ind;
			}
			ind++;
		}
		return -1;
	}


	template<class T>
	List<T> List<T>::operator*(const List<T>& list) const noexcept
	{
		List<T> temp;
		Data<T>* pos = this->first;
		while (pos)
		{
			if (list.find(pos->value) != -1 && temp.find(pos->value) == -1)
			{
				temp.push_back(pos->value);
			}
			pos = pos->next;
		}
		return temp;
	}

	template<class T>
	List<T> List<T>::operator/(const List<T>& list) const noexcept
	{
		List<T> temp;
		Data<T>* pos = this->first;
		while (pos)
		{
			if (list.find(pos->value) == -1 && temp.find(pos->value) == -1)
			{
				temp.push_back(pos->value);
			}
			pos = pos->next;
		}
		return temp;
	}