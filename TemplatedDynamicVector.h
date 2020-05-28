#include "repository_in_memory.h"
#pragma once

template <typename T>
class TemplatedDynamicVector
{
	private:
	T* elements;
	int size;
	int capacity;
	void resize();

public:
	TemplatedDynamicVector(int capacity);
	TemplatedDynamicVector& operator=(const TemplatedDynamicVector& vector);
	void add(T element);
	void remove(int position);
	T& operator[](int position);
	int getSize();
	int getCapacity();
	~TemplatedDynamicVector();

	class iterator
	{
	private:
		T* ptr;
		TemplatedDynamicVector& vector;

	public:
		iterator(T* pointer, TemplatedDynamicVector& container) : ptr{ pointer }, vector{ container }{}
		T& operator*();
		bool operator!=(const iterator& it);
		bool valid();
		iterator& operator=(const iterator& it);
		iterator operator++();
		iterator operator++(int);
	};

	typename iterator begin();
	typename iterator end();
};

template<typename T>
inline void TemplatedDynamicVector<T>::resize()
{
	this->capacity *= 2;
	auto aux = new T[this->capacity];
	for (int i = 0; i < this->size; i++)
		aux[i] = this->elements[i];
	delete[] this->elements;
	this->elements = aux;
}

template<typename T>
inline TemplatedDynamicVector<T>::TemplatedDynamicVector(int capacity)
{
	this->size = 0;
	this->capacity = capacity;
	this->elements = new T[this->capacity];
}

template<typename T>
inline TemplatedDynamicVector<T>& TemplatedDynamicVector<T>::operator=(const TemplatedDynamicVector& vector)
{
	if (this == &vector)
		return *this;

	this->size = vector.size;
	this->capacity = vector.capacity;
	delete[] this->elements;

	this->elements = new T[this->capacity];
	for (int i = 0; i < size; i++)
	{
		this->elements[i] = vector.elements[i];
	}

	return *this;
}

template<typename T>
inline void TemplatedDynamicVector<T>::add(T element)
{
	if (size == capacity)
		resize();

	this->elements[this->size++] = element;
}

template<typename T>
inline void TemplatedDynamicVector<T>::remove(int position)
{
	for (int i = position; i < this->size - 1; i++)
	{
		this->elements[i] = this->elements[i + 1];
	}
	this->size--;
}

template<typename T>
inline T& TemplatedDynamicVector<T>::operator[](int position)
{
	return this->elements[position];
}

template<typename T>
inline int TemplatedDynamicVector<T>::getSize()
{
	return this->size;
}

template<typename T>
inline int TemplatedDynamicVector<T>::getCapacity()
{
	return this->capacity;
}

template<typename T>
inline TemplatedDynamicVector<T>::~TemplatedDynamicVector()
{
	delete[] this->elements;
}

template<typename T>
inline typename TemplatedDynamicVector<T>::iterator TemplatedDynamicVector<T>::begin()
{
	return iterator(this->elements, *this);
}

template<typename T>
inline typename TemplatedDynamicVector<T>::iterator TemplatedDynamicVector<T>::end()
{
	return iterator(this->elements + this->size, *this);
}

template<typename T>
inline T& TemplatedDynamicVector<T>::iterator::operator*()
{
	return *this->ptr;
}

template<typename T>
inline bool TemplatedDynamicVector<T>::iterator::operator!=(const iterator& it)
{
	return this->ptr != it.ptr;
}

template<typename T>
inline bool TemplatedDynamicVector<T>::iterator::valid()
{
	if (ptr == (vector.end()).ptr)
		return false;
	return true;
}

template<typename T>
inline typename TemplatedDynamicVector<T>::iterator& TemplatedDynamicVector<T>::iterator::operator=(const iterator& it)
{
	this->ptr = it.ptr;
	this->vector = it.vector;
	return *this;
}

template<typename T>
inline typename TemplatedDynamicVector<T>::iterator TemplatedDynamicVector<T>::iterator::operator++()
{
	this->ptr++;
	return *this;
}

template<typename T>
inline typename TemplatedDynamicVector<T>::iterator TemplatedDynamicVector<T>::iterator::operator++(int)
{
	auto aux = *this;
	this->ptr++;
	return aux;
}
