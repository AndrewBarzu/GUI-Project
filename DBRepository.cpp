#include "DBRepository.h"
#include "setDebugNew.h"
#define new DEBUG_NEW

using namespace std;

void DBRepository::add(const Tower& tower)
{
	db.add(tower);
	this->elements.push_back(tower);
}

void DBRepository::remove(const std::string& location)
{
	db.remove(location);
	this->elements.erase(std::remove_if(this->elements.begin(), this->elements.end(), [location](const Tower& tower) { return tower.get_location() == location; }), this->elements.end());
}

void DBRepository::update(const Tower& tower)
{
	this->db.update(tower);
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	*it = tower;
}

Tower DBRepository::search(const std::string& location) const
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [location](const Tower& mytower) {return mytower.get_location() == location; });
	if (it != this->elements.end())
		return *it;
	return Tower();
}

int DBRepository::size() const
{
	return this->elements.size();
}

std::unique_ptr<RepoInterface::IteratorInterface> DBRepository::begin() const
{
	return make_unique<DBIterator>("first", this);
	// TODO: insert return statement here
}

std::unique_ptr<RepoInterface::IteratorInterface> DBRepository::end() const
{
	return make_unique<DBIterator>("last", this);
	// TODO: insert return statement here
}

DBRepository::DBIterator::DBIterator(std::string pos, const DBRepository* container) : IteratorInterface{ container }
{
	this->current = MyDatabase::iterator(&container->db, pos);
}

DBRepository::DBIterator::DBIterator(const DBIterator& other): IteratorInterface{container}
{
	this->current = other.current;
}

void DBRepository::DBIterator::first()
{
	this->current = MyDatabase::iterator(&((DBRepository*)this->container)->db, "first");
}

const Tower& DBRepository::DBIterator::getTower() const
{
	return *this->current;
}

void DBRepository::DBIterator::next()
{
	if (!this->valid())
		throw std::exception("Invalid next call");
	this->current++;
}

bool DBRepository::DBIterator::Equals(const std::unique_ptr<RepoInterface::IteratorInterface> it) const
{
	auto iter = dynamic_cast<DBRepository::DBIterator*>(const_cast<RepoInterface::IteratorInterface*>(it.get()));
	return this->current != iter->current;
}


bool DBRepository::DBIterator::valid() const
{
	return this->current.valid();
}
