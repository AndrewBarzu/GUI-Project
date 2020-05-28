#pragma once
#include <vector>
#include <string>
#include "domain.h"

class RepoInterface
{
public:
	class iterator
	{
	public:
		virtual const Tower& operator*() const = 0;
		virtual bool operator!=(const iterator& it) const = 0;
		virtual bool valid() const = 0;
		virtual iterator& operator=(const iterator& it) = 0;
		virtual iterator& operator++() = 0;
		virtual iterator& operator++(int) = 0;
	};

	virtual void add(const Tower& tower) = 0;
	virtual void remove(const std::string& location) = 0;
	virtual void update(const Tower& tower) = 0;
	virtual int size() const = 0;
	virtual typename iterator& begin() = 0;
	virtual typename iterator& end() = 0;
	virtual ~RepoInterface() {}
};

