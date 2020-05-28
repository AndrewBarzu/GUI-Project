#pragma once
#include "repository_in_file.h"

class HTMLRepo :
	public FileRepository
{
	class iterator : public FileRepository::iterator
	{
	private:
		std::string filename;
		Tower current_tower;
		HTMLRepo& repo;
	public:
		iterator(HTMLRepo& container) : repo{ container } {}
		iterator(std::string filename, HTMLRepo& container, std::string pos = "first");
		virtual const Tower& operator*() const override;
		virtual bool operator!=(const RepoInterface::iterator& it) const override;
		virtual bool operator!=(const iterator it) const;
		virtual bool valid() const override;
		virtual iterator& operator=(const RepoInterface::iterator& it) override;
		virtual iterator& operator=(const iterator& it);
		virtual iterator& operator++() override;
		virtual iterator& operator++(int) override;
		//virtual ~iterator();
	};
private:
	iterator last;
	iterator first;
	Tower read_tower(std::istream& is);
	void write_tower(std::ostream& os, const Tower& tower);
	void write_to_file(const std::vector<Tower>& towers);
public:
	HTMLRepo(std::string filename);
	void add(const Tower& tower) override;
	void remove(const std::string& location) override;
	void update(const Tower& tower) override;
	int size() const override;
	typename iterator& begin();
	typename iterator& end();
	~HTMLRepo() {};

};

