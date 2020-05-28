#pragma once
#include <winsqlite/winsqlite3.h>
#include <string>
#include "domain.h"
#include <exception>
#include <iostream>

class DBException: public std::exception
{
public:
	DBException(std::string error) 
		: std::exception{ error.c_str() } {}
};

class MyDatabase
{
private:
	sqlite3* db;
	std::string filename;
public:
	class iterator
	{
	private:
		sqlite3_stmt* current;
		Tower currentTower;
		const MyDatabase* db;

	public:
		iterator();
		iterator(const MyDatabase* db, std::string pos);
		iterator(const MyDatabase::iterator& it);
		const Tower& operator*() const;
		bool operator!=(const iterator& it) const;
		bool valid() const;
		iterator& operator=(const iterator& it);
		iterator operator++();
		iterator operator++(int);
		~iterator();
	};
	MyDatabase(const std::string& filename);
	void add(const Tower& tower);
	void remove(const std::string& location);
	void update(const Tower& tower);
	Tower search(std::string location);
	iterator begin();
	iterator end();
	std::vector<Tower> get_all();
	int size();
	~MyDatabase();
};

