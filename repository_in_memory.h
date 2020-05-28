#pragma once
#include "repository.h"

class TowerModel;

class RepoInMemory: public RepoInterface
{
public:
	class iterator : public RepoInterface::iterator
	{
	private:
		std::vector<Tower>::iterator ptr;
		std::vector<Tower>& repo;
	public:
		iterator(std::vector<Tower>::iterator ptr, RepoInMemory& container) : ptr{ ptr }, repo{ container.elements }{}
		const Tower& operator*() const override;
		bool operator!=(const RepoInterface::iterator& it) const override;
		bool operator!=(const iterator it) const;
		bool valid() const override;
		iterator& operator=(const RepoInterface::iterator& it) override;
		iterator& operator=(const iterator& it);
		iterator& operator++() override;
		iterator& operator++(int) override;
	};
private:
	std::vector<Tower> elements;
	iterator first;
	iterator last;
	friend class TowerModel;
public:

	RepoInMemory() : elements{ std::vector<Tower>() }, first{ iterator(elements.begin(), *this) }, last{ iterator(elements.end(), *this) } {}
	void add(const Tower& tower) override;
	void remove(const std::string& location) override;
	void update(const Tower& tower) override;
	int size() const override;
	typename iterator& begin();
	typename iterator& end();
	~RepoInMemory() {};
};

