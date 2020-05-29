#include "controller.h"
#include "validators.h"
#include <cstring>
#include <string>
#include <algorithm>
#include <qdebug.h>
#include "setDebugNew.h"
#define new DEBUG_NEW

using namespace std;

/// adds a new tower. If no addition happened, it returns a 1, else a 0. Also, if the mode is wrong, it just returns with a 0
void Controller::add(string location, string size, string auraLevel, string separateParts, string vision)
{
	if (mode != "A")
		throw ModeException("Wrong mode!");
	vector<string> params;
	params.push_back(location);
	params.push_back(size);
	params.push_back(auraLevel);
	params.push_back(separateParts);
	params.push_back(vision);
	/*params.push_back(auraLevel);
	params.push_back(separateParts);
	params.push_back(size);
	params.push_back(location);
	params.push_back(vision);*/
	Tower tower(params);
	Helper::tower_validator(tower);
	repository->add(tower);
	this->undoStack.push_back(make_unique<AddAction>(this->repository, tower));
	this->redoStack.clear();
}

/// removes a tower. If no remove happened, it returns a 1, else a 0. Also, if the mode is wrong, it just returns with a 0
void Controller::remove(string location)
{
	if (mode != "A")
		throw ModeException("Wrong mode!");
	Tower tower = this->repository->search(location);
	repository->remove(location);
	bool removed = true;
	try
	{
		this->saved->remove(location);
	}
	catch (const std::exception& e)
	{
		removed = false;
	}
	if (removed)
		this->undoStack.push_back(make_unique<RemoveAction>(this->repository, tower, this->saved));
	else
		this->undoStack.push_back(make_unique<RemoveAction>(this->repository, tower));
	this->redoStack.clear();
}

/// updates a tower. If no update happened, it returns a 1, else a 0. Also, if the mode is wrong, it just returns with a 0
void Controller::update(string location, string size, string auraLevel, string separateParts, string vision)
{
	if (mode != "A")
		throw ModeException("Wrong mode!");
	vector<string> params;
	params.push_back(location);
	params.push_back(size);
	params.push_back(auraLevel);
	params.push_back(separateParts);
	params.push_back(vision);
	/*params.push_back(auraLevel);
	params.push_back(separateParts);
	params.push_back(size);
	params.push_back(location);
	params.push_back(vision);*/
	Tower tower(params);
	Helper::tower_validator(tower);
	Tower oldTower = this->repository->search(location);
	repository->update(tower);
	bool updated = true;
	try
	{
		this->saved->update(tower);
	}
	catch(const std::exception& e)
	{
		updated = false;
	}
	if (updated)
		this->undoStack.push_back(make_unique<UpdateAction>(this->repository, oldTower, tower, this->saved));
	else
		this->undoStack.push_back(make_unique<UpdateAction>(this->repository, oldTower, tower));
	this->redoStack.clear();
}

/// changes the controller mode. If no change, then it returns a 1, else 0
int Controller::change_mode(std::string mode)
{
	if (this->mode != mode)
	{
		this->mode = mode;
		iterator = repository->begin();
		return 0;
	}
	return 1;}

/// Returns the first entry having the given size. If the mode is wrong, returns a 0. If there is no entry with that size, also returns a 0
/// If size is the empty string, no filtering is done
std::string Controller::list(std::string size)
{
	if (mode != "B") throw ModeException("Wrong mode!");
	this->sizeFilter = size;
	iterator = repository->begin();
	if (size != "")
	{
		while (iterator->valid() && iterator->getTower().get_size() != this->sizeFilter)
		{
			iterator->next();
		}
	}
	if (iterator->valid())
		return iterator->getTower().print();
	else
		return "";
}

std::vector<Tower> Controller::print(std::string size) const
{
	std::vector<Tower> vect;
	if (size == "")
		return this->repository->getAll();
	for (auto tower = this->repository->getAll().begin(); tower != this->repository->getAll().end(); tower++)
	{
		if (size == "" || tower->get_size() == size)
		{
			vect.push_back(*tower);
		}
	}
	return vect;
}

void Controller::save(std::string location)
{
	Tower tower = this->repository->search(location);
	if (tower.get_location() == location)
	{
		try
		{
			saved->add(tower);
			this->undoStack.push_back(make_unique<AddAction>(this->saved, tower));
		}
		catch (const std::exception& e)
		{
			;
		}
		return;
	}
	throw std::exception("No tower at given location!");}

const std::vector<Tower>& Controller::getSaved() const
{
	return this->saved->getAll();
}

/// Returns the next entry having a size given in the list command. If the current element is the last, then next goes back to the first one
/// If the mode is wrong, returns a 0
std::string Controller::next()
{
	if (mode != "B") throw ModeException("Wrong mode!");
	if (this->sizeFilter == "" && iterator->valid())
	{
		iterator->next();
	}
	else if (this->sizeFilter != "" && iterator->valid())
	{
		iterator->next();
		while (iterator->valid() && iterator->getTower().get_size() != this->sizeFilter)
		{
			iterator->next();
		}
	}

	if (!iterator->valid())
	{
		std::string res = this->list(this->sizeFilter);
		return res;
	}
	return this->iterator->getTower().print();
}

/// Returns the current mode in which the controller operates
std::string Controller::getMode()
{
	return this->mode;
}

void Controller::undo()
{
	if (this->undoStack.empty())
		throw std::exception("No more undo's!");
	auto action = move(this->undoStack.back());
	this->undoStack.pop_back();
	action->undoAction();
	this->redoStack.push_back(move(action));
}

void Controller::redo()
{
	if (this->redoStack.empty())
		throw std::exception("No more redo's!");
	auto action = move(this->redoStack.back());
	this->redoStack.pop_back();
	action->doAction();
	this->undoStack.push_back(move(action));
}

/// Saves the tower that is at the given location
/// If no tower resides there, returns a 1
//int Controller::save(std::string location)
//{
//	std::vector<std::string> params;
//	params.push_back(location);
//	return this->repository.save(params);
//}



