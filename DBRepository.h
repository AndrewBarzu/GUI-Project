#pragma once
#include "repository.h"
#include "MyDatabase.h"


class DBRepository :
	public RepoInterface
{
public:
	class iterator : public RepoInterface::iterator
	{
	protected:
		MyDatabase::iterator current;
		DBRepository& repo;
	public:
		//iterator() {}
		iterator(std::string pos, DBRepository& container);
		virtual const Tower& operator*() const override;
		virtual bool operator!=(const RepoInterface::iterator& it) const override;
		virtual bool operator!=(const iterator it) const;
		virtual bool valid() const override;
		virtual iterator& operator=(const RepoInterface::iterator& it) override;
		virtual iterator& operator=(const iterator& it);
		virtual iterator& operator++() override;
		virtual iterator& operator++(int) override;
		virtual ~iterator() {}
	};
protected:
	MyDatabase db;
	int mySize;
private:
	iterator first;
	iterator last;
public:
	DBRepository(std::string dbname) : db{ dbname }, first{ iterator("first", *this) }, last{ iterator("last", *this) }
	{
		this->mySize = db.size();
	}
	virtual void add(const Tower& tower) override;
	virtual void remove(const std::string& location) override;
	virtual void update(const Tower& tower) override;
	virtual int size() const override;
	virtual typename iterator& begin();
	virtual typename iterator& end();
	virtual ~DBRepository() {};
};

