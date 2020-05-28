#include "repository_in_memory.h"
#include "domain.h"
#include <algorithm>
#include <exception>
#include <typeinfo>
#include "setDebugNew.h"
#define new DEBUG_NEW

/// Adds a new tower to the repo
/// Returns a 1 if the location is not unique (not a success), 0 otherwise
void RepoInMemory::add(const Tower& tower)
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	/*for (auto& const it: this->elements)
	{
		if (it.get_location() == params[3])
			return 1;
	}*/
	if (it != this->elements.end())
		throw std::exception("Element already exists!");
	elements.push_back(tower);
}

/// Removes a tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void RepoInMemory::remove(const std::string& location)
{
	bool flag = true;
	int oldSize = this->elements.size();
	this->elements.erase(std::remove_if(this->elements.begin(), this->elements.end(), [location](const Tower& tower) { return tower.get_location() == location; }), this->elements.end());
	if (this->elements.size() == oldSize)
		throw std::exception("Tried to remove a non existing element!");
}

/// Updates a new tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void RepoInMemory::update(const Tower& tower)
{
	bool flag = true;
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	if (it != this->elements.end())
	{
		*it = tower;
		return;
	}
	/*for (int i = 0; i < elements.size() && flag; i++)
	{
		if (strcmp(this->elements[i].get_location(), params[3]) == 0)
		{
			Tower new_tower = Tower(params);
			elements[i] = new_tower;
			return 0;
		}
	}*/
	throw std::exception("Tried to update a non existing element!");}

/// Returns the size of the repo
int RepoInMemory::size() const
{
	return elements.size();
}

typename RepoInMemory::iterator& RepoInMemory::begin()
{
	// TODO: insert return statement here
	//this->first = iterator(elements.begin(), *this);
	this->first = iterator(elements.begin(), *this);
	return this->first;
}

typename RepoInMemory::iterator& RepoInMemory::end()
{
	// TODO: insert return statement here
	//this->last = iterator(elements.end(), *this);
	this->last = iterator(elements.end(), *this);
	return this->last;
}

const Tower& RepoInMemory::iterator::operator*() const
{
	// TODO: insert return statement here
	return *(this->ptr);
}

bool RepoInMemory::iterator::operator!=(const RepoInterface::iterator& it) const
{
	RepoInMemory::iterator& iter = dynamic_cast<RepoInMemory::iterator&>(const_cast<RepoInterface::iterator&>(it));
	return this->ptr != iter.ptr;
}

bool RepoInMemory::iterator::operator!=(const iterator it) const
{
	return this->ptr != it.ptr;
}

bool RepoInMemory::iterator::valid() const
{
	return this->ptr != this->repo.end();
}

typename RepoInMemory::iterator& RepoInMemory::iterator::operator++()
{
	// TODO: insert return statement here
	this->ptr++;
	return *this;
}

typename RepoInMemory::iterator& RepoInMemory::iterator::operator++(int c)
{
	// TODO: insert return statement here
	auto aux = *this;
	this->ptr++;
	return aux;
}

RepoInMemory::iterator& RepoInMemory::iterator::operator=(const RepoInterface::iterator& it) {
	RepoInMemory::iterator& iter = dynamic_cast<RepoInMemory::iterator&>(const_cast<RepoInterface::iterator&>(it));
	this->ptr = iter.ptr;
	this->repo = iter.repo;
	return *this;
}

RepoInMemory::iterator& RepoInMemory::iterator::operator=(const iterator& it)
{
	this->ptr = it.ptr;
	this->repo = it.repo;
	return *this;
}
