#pragma once
#include <vector>
#include <string>
#include <memory>
#include "domain.h"
#include "domain.h"

class RepoInterface
{
protected:
	std::vector<Tower> elements;
public:
	class IteratorInterface
	{
	protected:
		const RepoInterface* container;
		Tower currentTower;
	public:
		IteratorInterface(const RepoInterface* container) : container{ container } {}
		IteratorInterface(const IteratorInterface& other): container{other.container}, currentTower{other.currentTower} {}
		virtual const Tower& getTower() const = 0;
		virtual void first() = 0;
		virtual bool Equals(const std::unique_ptr<IteratorInterface> it) const = 0;
		virtual bool valid() const = 0;
		virtual void next() = 0;
		virtual ~IteratorInterface() {}
	};

	virtual void add(const Tower& tower) = 0;
	virtual void remove(const std::string& location) = 0;
	virtual void update(const Tower& tower) = 0;
	virtual Tower search(const std::string& location) const = 0;
	int size() const { return this->elements.size(); }
	virtual std::unique_ptr<IteratorInterface> begin() const = 0;
	virtual std::unique_ptr<IteratorInterface> end() const = 0;
	const std::vector<Tower>& getAll() const { return this->elements; }
	virtual ~RepoInterface() {}
};

