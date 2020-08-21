#pragma once
#include "repository_in_file.h"

class HTMLRepo :
	public FileRepository
{
	class HTMLIterator : public RepoInterface::IteratorInterface
	{
	private:
		std::string filename;
	public:
		HTMLIterator(const HTMLRepo* container, std::string filename, std::string pos = "first");
		//HTMLIterator(const std::unique_ptr<IteratorInterface> other);
		virtual void first() override;
		virtual const Tower& getTower() const override;
		virtual bool valid() const override;
		virtual bool Equals(const std::unique_ptr<IteratorInterface> it) const override;
		virtual void next() override;
		virtual ~HTMLIterator() {}
	};
private:
	Tower read_tower(std::istream& is);
	void write_tower(std::ostream& os, const Tower& tower);
	void write_to_file(const std::vector<Tower>& towers);
public:
	HTMLRepo(std::string filename);
	void add(const Tower& tower) override;
	void remove(const std::string& location) override;
	void update(const Tower& tower) override;
	Tower search(const std::string& location) const override;
	std::unique_ptr<RepoInterface::IteratorInterface> begin() const override;
	std::unique_ptr<RepoInterface::IteratorInterface> end() const override;
	~HTMLRepo() {};

};

