#pragma once
#include "repository_in_memory.h"
#include "repository_in_file.h"
#include <string>
#include <vector>
#include <memory>
#include "Action.h"

class ModeException : public std::exception
{
public:
	ModeException(std::string e)
		: std::exception{ e.c_str() }{}
};

class Controller
{
private:
	RepoInterface* repository;
	std::unique_ptr<RepoInterface::IteratorInterface> iterator;
	std::string sizeFilter;
	std::string mode;
	RepoInterface* saved;
	std::vector<std::unique_ptr<Action>> undoStack;
	std::vector<std::unique_ptr<Action>> redoStack;

public:
	Controller(RepoInterface* repository, FileRepository* list) : mode{ 'A' }, iterator{ repository->begin() }, sizeFilter{ "" }, repository{ repository }, saved{ list }{}
	Controller(const Controller& other): mode{other.mode}, iterator{other.repository->begin()}, sizeFilter{other.sizeFilter}, repository{other.repository}, saved{other.saved} {}
	void add(std::string location, std::string size, std::string auraLevel, std::string separateParts, std::string vision);
	void remove(std::string location);
	void update(std::string location, std::string size, std::string auraLevel, std::string separateParts, std::string vision);
	int size() const { return this->repository->size(); }
	virtual void load()
	{
		this->add("yey", "chonk", "200", "700", "lel");
		this->add("yfy", "big", "100", "300", "kek");
		this->add("ygy", "big", "101", "300", "kek");
		this->add("yhy", "big", "101", "302", "kek");
		this->add("yiy", "big", "102", "302", "kek");
		this->add("yjy", "big", "102", "302", "kek");
		this->add("yky", "big", "103", "304", "kek");
		this->add("yly", "big", "103", "304", "kek");
		this->add("ymy", "big", "104", "304", "kek");
	}
	int change_mode(std::string mode);
	std::string list(std::string size);
	void mylist() { system(((FileRepository*)this->saved)->get_path().c_str()); }
	std::vector<Tower> print(std::string size = "") const;
	void save(std::string location);
	std::vector<Tower> getSaved() const;
	std::string next();
	std::string getMode();
	void undo();
	void redo();
};

