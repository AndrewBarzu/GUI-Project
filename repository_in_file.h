#pragma once
#include "repository.h"
#include <fstream>


class FileRepository :
	public RepoInterface
{
public:
	class iterator;
	class iterator : public RepoInterface::iterator
	{
	protected:
		std::string filename;
		Tower current_tower;
	public:
		iterator(){}
		iterator(std::string filename, std::string pos = "first");
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
	std::string filename;
	int mySize;
private:
	iterator first;
	iterator last;
public:
	FileRepository(std::string filename) : filename{ filename }, mySize{ 0 }, first{ iterator(filename) }, last{ iterator(filename, "last") }
	{
		std::fstream fs;
		fs.open(filename, std::ios::app);
		fs.close();
	}
	virtual void add(const Tower& tower) override;
	virtual void remove(const std::string& location) override;
	virtual void update(const Tower& tower) override;
	virtual int size() const override;
	virtual typename iterator& begin();
	virtual typename iterator& end();
	const std::string get_path() { return this->filename; }
	virtual ~FileRepository() {};
};

