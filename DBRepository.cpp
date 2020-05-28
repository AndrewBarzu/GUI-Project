#include "DBRepository.h"
#include "setDebugNew.h"
#define new DEBUG_NEW

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

int DBRepository::size() const
{
	return this->mySize;
}

typename DBRepository::iterator& DBRepository::begin()
{
	this->first = DBRepository::iterator("first", *this);
	return this->first;
	// TODO: insert return statement here
}

typename DBRepository::iterator& DBRepository::end()
{
	this->last = DBRepository::iterator("last", *this);
	return this->last;
	// TODO: insert return statement here
}

DBRepository::iterator::iterator(std::string pos, DBRepository& container) : repo{ container }
{
	this->current = MyDatabase::iterator(&(this->repo.db), pos);
}

const Tower& DBRepository::iterator::operator*() const
{
	return *this->current;
}

DBRepository::iterator& DBRepository::iterator::operator++()
{
	++this->current;
	return *this;
}

DBRepository::iterator& DBRepository::iterator::operator++(int c)
{
	auto aux = *this;
	++this->current;
	return aux;
}

bool DBRepository::iterator::operator!=(const RepoInterface::iterator& it) const
{
	DBRepository::iterator& iter = dynamic_cast<DBRepository::iterator&>(const_cast<RepoInterface::iterator&>(it));
	return this->current!= iter.current;
}

bool DBRepository::iterator::operator!=(const iterator it) const
{
	return this->current != it.current;
}

bool DBRepository::iterator::valid() const
{
	return this->current.valid();
}

DBRepository::iterator& DBRepository::iterator::operator=(const RepoInterface::iterator& it) {
	DBRepository::iterator& iter = dynamic_cast<DBRepository::iterator&>(const_cast<RepoInterface::iterator&>(it));
	this->current = iter.current;
	return *this;
}

DBRepository::iterator& DBRepository::iterator::operator=(const iterator& it)
{
	this->current = it.current;
	return *this;
}