#include "controller.h"
#include "validators.h"
#include <cstring>
#include <string>
#include <algorithm>
#include <qdebug.h>
#include "setDebugNew.h"
#define new DEBUG_NEW

/// adds a new tower. If no addition happened, it returns a 1, else a 0. Also, if the mode is wrong, it just returns with a 0
void Controller::add(std::string location, std::string size, std::string auraLevel, std::string separateParts, std::string vision)
{
	if (mode != "A")
		throw ModeException("Wrong mode!");
	std::vector<std::string> params;
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
}

/// removes a tower. If no remove happened, it returns a 1, else a 0. Also, if the mode is wrong, it just returns with a 0
void Controller::remove(std::string location)
{
	if (mode != "A")
		throw ModeException("Wrong mode!");
	repository->remove(location);
	try
	{
		this->saved->remove(location);
	}
	catch (const std::exception& e)
	{
		;
	}
}

/// updates a tower. If no update happened, it returns a 1, else a 0. Also, if the mode is wrong, it just returns with a 0
void Controller::update(std::string location, std::string size, std::string auraLevel, std::string separateParts, std::string vision)
{
	if (mode != "A")
		throw ModeException("Wrong mode!");
	std::vector<std::string> params;
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
	repository->update(tower);
	try
	{
		this->saved->update(tower);
	}
	catch(const std::exception& e)
	{
		;
	}
	//std::replace_if(saved.begin(), saved.end(), [location](const Tower& t) { return t.get_location() == location; }, Tower(params));
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
		while (iterator.valid() && (*iterator).get_size() != this->sizeFilter)
		{
			iterator++;
		}
	}
	if (iterator.valid())
		return (*iterator).print();
	else
		return "";}

std::vector<Tower> Controller::print(std::string size) const
{
	std::vector<Tower> vect;
	for (auto& tower = this->repository->begin(); tower != this->repository->end(); ++tower)
	{
		if (size == "" || (*tower).get_size() == size)
		{
			vect.push_back(*tower);
		}
	}
	return vect;
}

void Controller::save(std::string location)
{
	for (auto& iter = this->repository->begin(); iter.valid(); iter++)
	{
		if ((*(iter)).get_location() == location)
		{
			try
			{
				saved->add(*iter);
			}
			catch (const std::exception& e)
			{
				;
			}
			return;
		}
	}
	throw std::exception("No tower at given location!");}

std::vector<Tower> Controller::getSaved() const
{
	std::vector<Tower> vect;
	for (auto& tower = this->saved->begin(); tower != this->saved->end(); ++tower)
	{
		vect.push_back(*tower);
	}
	return vect;
}

/// Returns the next entry having a size given in the list command. If the current element is the last, then next goes back to the first one
/// If the mode is wrong, returns a 0
std::string Controller::next()
{
	if (mode != "B") throw ModeException("Wrong mode!");
	if (this->sizeFilter == "" && iterator.valid())
	{
		iterator++;
	}
	else if (this->sizeFilter != "" && iterator.valid())
	{
		iterator++;
		while (iterator.valid() && (*iterator).get_size() != this->sizeFilter)
		{
			iterator++;
		}
	}

	if (!iterator.valid())
	{
		std::string res = this->list(this->sizeFilter);
		return res;
	}
	return (*(this->iterator)).print();
}

/// Returns the current mode in which the controller operates
std::string Controller::getMode()
{
	return this->mode;
}

/// Saves the tower that is at the given location
/// If no tower resides there, returns a 1
//int Controller::save(std::string location)
//{
//	std::vector<std::string> params;
//	params.push_back(location);
//	return this->repository.save(params);
//}



