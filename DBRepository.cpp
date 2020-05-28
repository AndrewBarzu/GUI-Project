#include "DBRepository.h"
#include "setDebugNew.h"
#define new DEBUG_NEW

using namespace std;

void DBRepository::add(const Tower& tower)
{
	db.add(tower);
	this->mySize += 1;
}

void DBRepository::remove(const std::string& location)
{
	db.remove(location);
	this->mySize -= 1;
}

void DBRepository::update(const Tower& tower)
{
	this->db.update(tower);
}

Tower DBRepository::search(const std::string& location) const
{
	for (auto it = this->begin(); it->valid(); it->next())
		if (it->getTower().get_location() == location)
			return it->getTower();
	return Tower();
}

int DBRepository::size() const
{
	return this->mySize;
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
