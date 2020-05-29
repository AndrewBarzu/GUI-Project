#pragma once
#include "repository.h"

class TowerModel;

class RepoInMemory: public RepoInterface
{
public:
	class InMemoryIterator : public RepoInterface::IteratorInterface
	{
	private:
		std::vector<Tower>::const_iterator ptr;
	public:
		InMemoryIterator(const RepoInMemory* container, std::vector<Tower>::const_iterator ptr) : IteratorInterface{ container }, ptr{ ptr }{}
		InMemoryIterator(const InMemoryIterator& other);
		virtual void first() override;
		virtual const Tower& getTower() const override;
		virtual bool valid() const override;
		virtual bool Equals(const std::unique_ptr<IteratorInterface> it) const override;
		virtual void next() override;
		~InMemoryIterator() {}
	};
private:
	friend class TowerModel;
public:

	RepoInMemory() {}
	void add(const Tower& tower) override;
	void remove(const std::string& location) override;
	void update(const Tower& tower) override;
	Tower search(const std::string& location) const override;
	int size() const override;
	std::unique_ptr<IteratorInterface> begin() const override;
	std::unique_ptr<IteratorInterface> end() const override;
	~RepoInMemory() {};
};

