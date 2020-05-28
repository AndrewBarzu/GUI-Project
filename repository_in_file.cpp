#include "repository_in_file.h"
#include "domain.h"
#include <algorithm>
#include <exception>
#include <typeinfo>
#include "setDebugNew.h"
#define new DEBUG_NEW

/// Adds a new tower to the repo
/// Returns a 1 if the location is not unique (not a success), 0 otherwise
void FileRepository::add(const Tower& tower)
{
	std::ifstream is;
	is.open(this->filename, std::ios_base::in);
	Tower readTower;
	while (!is.eof())
	{
		is >> readTower;
		if (readTower.get_location() == tower.get_location())
			throw std::exception("Element already exists!");
	}
	is.close();
	std::ofstream os;
	os.open(this->filename, std::ios_base::app);
	os << tower << '\n';
	os.close();
	this->mySize++;
}

/// Removes a tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void FileRepository::remove(const std::string& location)
{
	std::ifstream is;
	is.open(this->filename, std::ios_base::in);
	std::vector<Tower> temp;
	Tower tower;
	while (!is.eof())
	{
		is >> tower;
		if (is.eof())
			break;
		temp.push_back(tower);
	}
	is.close();
	int size;
	size = temp.size();
	temp.erase(std::remove_if(temp.begin(), temp.end(), [location](const Tower& tower) { return tower.get_location() == location; }), temp.end());
	if (temp.size() == size)
		throw std::exception("Tried to remove a non existing element!");
	std::ofstream os;
	os.open(this->filename, std::ios_base::out);
	for (const auto& tower : temp)
	{
		os << tower << '\n';
	}
	os.close();
	this->mySize--;
	/*bool flag = true;
	int oldSize = this->elements.size();
	this->elements.erase(std::remove_if(this->elements.begin(), this->elements.end(), [params](const Tower& tower) { return tower.get_location() == params[0]; }), this->elements.end());
	if (this->elements.size() == oldSize)
		return 1;*/
}

/// Updates a new tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void FileRepository::update(const Tower& tower)
{
	bool flag = true;
	std::ifstream is;
	is.open(this->filename, std::ios_base::in);
	std::vector<Tower> temp;
	Tower readTower;
	while (!is.eof())
	{
		is >> readTower;
		if (is.eof())
			break;
		temp.push_back(readTower);
	}
	is.close();

	auto it = std::find_if(temp.begin(), temp.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	if (it == temp.end())
		throw std::exception("Tried to update a non existing element!");

	*it = tower;
	std::ofstream os;
	os.open(this->filename, std::ios_base::out);
	for (const auto& tower : temp)
	{
		os << tower << '\n';
	}
	os.close();
	/*for (int i = 0; i < elements.size() && flag; i++)
	{
		if (strcmp(this->elements[i].get_location(), params[3]) == 0)
		{
			Tower new_tower = Tower(params);
			elements[i] = new_tower;
			return 0;
		}
	}*/
}

/// Returns the size of the repo
int FileRepository::size() const
{
	return this->mySize;
}

typename FileRepository::iterator& FileRepository::begin()
{
	// TODO: insert return statement here
	//this->first = iterator(elements.begin(), *this);
	this->first = iterator(this->filename);
	return this->first;
}

typename FileRepository::iterator& FileRepository::end()
{
	// TODO: insert return statement here
	//this->last = iterator(elements.end(), *this);
	this->last = iterator(this->filename, "last");
	return this->last;
}


FileRepository::iterator::iterator(std::string filename, std::string pos)
{
	this->filename = filename;
	if (pos == "first")
	{
		std::ifstream is;
		std::fstream fs;
		fs.open(this->filename, std::ios::out | std::ios::app);
		fs.close();
		is.open(filename);
		Tower temp;
		is >> temp;
		if (is.eof())
		{
			this->current_tower = Tower();
		}
		else
		{
			this->current_tower = temp;
		}
		is.close();
	}
	else
	{
		this->current_tower = Tower();
	}
}

const Tower& FileRepository::iterator::operator*() const
{
	// TODO: insert return statement here
	return this->current_tower;
}

bool FileRepository::iterator::operator!=(const RepoInterface::iterator& it) const
{
	FileRepository::iterator& iter = dynamic_cast<FileRepository::iterator&>(const_cast<RepoInterface::iterator&>(it));
	return this->current_tower != iter.current_tower;
}

bool FileRepository::iterator::operator!=(const iterator it) const
{
	return this->current_tower != it.current_tower;
}

bool FileRepository::iterator::valid() const
{
	return this->current_tower != Tower();
}

typename FileRepository::iterator& FileRepository::iterator::operator++()
{
	// TODO: insert return statement here
	std::ifstream is;
	std::fstream fs;
	fs.open(this->filename, std::ios::out | std::ios::app);
	fs.close();
	is.open(filename, std::ios_base::in);
	Tower temp;
	bool eof = false;
	while (!is.eof())
	{
		if (temp == this->current_tower)
			break;
		is >> temp;
		if (is.eof())
		{
			eof = true;
			break;
		}
	}
	is >> temp;
	if (is.eof())
		eof = true;
	is.close();
	if (eof)
	{
		this->current_tower = Tower();
	}
	else
		this->current_tower = temp;
	return *this;
}

typename FileRepository::iterator& FileRepository::iterator::operator++(int c)
{
	auto aux = *this;
	std::ifstream is;
	std::fstream fs;
	fs.open(this->filename, std::ios::out | std::ios::app);
	fs.close();
	is.open(filename, std::ios_base::in);
	Tower temp;
	bool eof = false;
	while (!is.eof())
	{
		if (temp == this->current_tower)
			break;
		is >> temp;
		if (is.eof())
		{
			eof = true;
			break;
		}
	}
	is >> temp;
	if (is.eof())
		eof = true;
	is.close();
	if (eof)
	{
		this->current_tower = Tower();
	}
	else
		this->current_tower = temp;
	return aux;
}

FileRepository::iterator& FileRepository::iterator::operator=(const RepoInterface::iterator& it) {
	FileRepository::iterator& iter = dynamic_cast<FileRepository::iterator&>(const_cast<RepoInterface::iterator&>(it));
	this->current_tower = iter.current_tower;
	this->filename = iter.filename;
	return *this;
}

FileRepository::iterator& FileRepository::iterator::operator=(const iterator& it)
{
	this->filename = it.filename;
	this->current_tower = it.current_tower;
	return *this;
}
