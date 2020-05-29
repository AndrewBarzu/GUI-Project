#include "repository_in_memory.h"
#include "domain.h"
#include <algorithm>
#include <exception>
#include <typeinfo>
#include "setDebugNew.h"
#define new DEBUG_NEW

using namespace std;

/// Adds a new tower to the repo
/// Returns a 1 if the location is not unique (not a success), 0 otherwise
void RepoInMemory::add(const Tower& tower)
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	if (it != this->elements.end())
		throw std::exception("Element already exists!");
	elements.push_back(tower);
}

/// Removes a tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void RepoInMemory::remove(const std::string& location)
{
	int oldSize = this->elements.size();
	this->elements.erase(std::remove_if(this->elements.begin(), this->elements.end(), [location](const Tower& tower) { return tower.get_location() == location; }), this->elements.end());
	if (this->elements.size() == oldSize)
		throw std::exception("Tried to remove a non existing element!");
}

/// Updates a new tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void RepoInMemory::update(const Tower& tower)
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	if (it != this->elements.end())
	{
		*it = tower;
		return;
	}
	throw std::exception("Tried to update a non existing element!");
}

Tower RepoInMemory::search(const std::string& location) const
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [location](const Tower& mytower) {return mytower.get_location() == location; });
	if (it != this->elements.end())
		return *it;
	return Tower();
}

/// Returns the size of the repo
int RepoInMemory::size() const
{
	return elements.size();
}

unique_ptr<RepoInterface::IteratorInterface> RepoInMemory::begin() const
{
	// TODO: insert return statement here
	//this->first = iterator(elements.begin(), *this);
	return make_unique<InMemoryIterator>(this, this->elements.cbegin());
}

unique_ptr<RepoInterface::IteratorInterface> RepoInMemory::end() const
{
	// TODO: insert return statement here
	//this->last = iterator(elements.end(), *this);
	return make_unique<InMemoryIterator>(this, this->elements.cend());
}

RepoInMemory::InMemoryIterator::InMemoryIterator(const InMemoryIterator& other): IteratorInterface{other.container}
{
	this->ptr = other.ptr;
}

void RepoInMemory::InMemoryIterator::first()
{
	this->ptr = ((RepoInMemory*)this->container)->elements.begin();
}

const Tower& RepoInMemory::InMemoryIterator::getTower() const
{
	// TODO: insert return statement here
	return *(this->ptr);
}

bool RepoInMemory::InMemoryIterator::Equals(const unique_ptr<RepoInterface::IteratorInterface> it) const
{
	auto ptr = dynamic_cast<RepoInMemory::InMemoryIterator*>(it.get());
	if (ptr == nullptr)
		return false;
	return this->getTower() != it->getTower();
}

bool RepoInMemory::InMemoryIterator::valid() const
{
	return this->ptr != ((RepoInMemory*)this->container)->elements.end();
}

void RepoInMemory::InMemoryIterator::next()
{
	// TODO: insert return statement here
	this->ptr++;
}

