#pragma once
#include "repository.h"
#include <fstream>


class FileRepository :
	public RepoInterface
{
public:
	class FileIterator : public RepoInterface::IteratorInterface
	{
	protected:
		std::string filename;
	public:
		FileIterator(const FileRepository* container, std::string filename, std::string pos = "first");
		FileIterator(const FileIterator& other);
		virtual void first() override;
		virtual const Tower& getTower() const override;
		virtual bool valid() const override;
		virtual bool Equals(const std::unique_ptr<IteratorInterface> it) const override;
		virtual void next() override;
		virtual ~FileIterator() {}
	};
protected:
	std::string filename;
	int mySize;
public:
	FileRepository(std::string filename) : filename{ filename }, mySize{ 0 }
	{
		std::fstream fs;
		fs.open(filename, std::ios::app);
		fs.close();
	}
	virtual void add(const Tower& tower) override;
	virtual void remove(const std::string& location) override;
	virtual void update(const Tower& tower) override;
	Tower search(const std::string& location) const override;
	virtual int size() const override;
	virtual std::unique_ptr<IteratorInterface> begin() const override;
	virtual std::unique_ptr<IteratorInterface> end() const override;
	const std::string get_path() { return this->filename; }
	virtual ~FileRepository() {};
};

