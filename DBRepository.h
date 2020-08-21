#pragma once
#include "repository.h"
#include "MyDatabase.h"


class DBRepository :
	public RepoInterface
{
public:
	class DBIterator : public RepoInterface::IteratorInterface
	{
	protected:
		MyDatabase::iterator current;
	public:
		DBIterator(std::string pos, const DBRepository* container);
		DBIterator(const DBIterator& other);
		virtual void first() override;
		virtual const Tower& getTower() const override;
		virtual bool valid() const override;
		virtual bool Equals(const std::unique_ptr<IteratorInterface> it) const override;
		virtual void next() override;
		virtual ~DBIterator() {}
	};
protected:
	MyDatabase db;
	int mySize;
public:
	DBRepository(std::string dbname) : db{ dbname }
	{
		this->elements = this->db.get_all();
		this->mySize = this->elements.size();
	}
	virtual void add(const Tower& tower) override;
	virtual void remove(const std::string& location) override;
	virtual void update(const Tower& tower) override;
	Tower search(const std::string& location) const override;
	virtual std::unique_ptr<RepoInterface::IteratorInterface> begin() const override;
	virtual std::unique_ptr<RepoInterface::IteratorInterface> end() const override;
	virtual ~DBRepository() {};
};

