#include "repository_in_file.h"
#include "domain.h"
#include <algorithm>
#include <exception>
#include <typeinfo>
#include "setDebugNew.h"
#define new DEBUG_NEW

using namespace std;

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

Tower FileRepository::search(const std::string& location) const
{
	for (auto it = this->begin(); it->valid(); it->next())
		if (it->getTower().get_location() == location)
			return it->getTower();
	return Tower();
}

/// Returns the size of the repo
int FileRepository::size() const
{
	return this->mySize;
}

unique_ptr<RepoInterface::IteratorInterface> FileRepository::begin() const
{
	// TODO: insert return statement here
	//this->first = iterator(elements.begin(), *this);
	return make_unique<FileIterator>(this, this->filename);
}

unique_ptr<RepoInterface::IteratorInterface> FileRepository::end() const
{
	// TODO: insert return statement here
	//this->last = iterator(elements.end(), *this);
	return make_unique<FileIterator>(this, this->filename, "last");
}


FileRepository::FileIterator::FileIterator(const FileRepository* container, std::string filename, std::string pos): 
	IteratorInterface{ container }
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
			this->currentTower = Tower();
		}
		else
		{
			this->currentTower = temp;
		}
		is.close();
	}
	else
	{
		this->currentTower = Tower();
	}
}

FileRepository::FileIterator::FileIterator(const FileIterator& other): IteratorInterface{other.container}
{
	this->currentTower = other.currentTower;
	this->filename = other.filename;
}

void FileRepository::FileIterator::first()
{
	std::ifstream is;
	std::fstream fs;
	fs.open(this->filename, std::ios::out | std::ios::app);
	fs.close();
	is.open(this->filename);
	Tower temp;
	is >> temp;
	if (is.eof())
	{
		this->currentTower = Tower();
	}
	else
	{
		this->currentTower = temp;
	}
	is.close();
}

const Tower& FileRepository::FileIterator::getTower() const
{
	// TODO: insert return statement here
	return this->currentTower;
}

bool FileRepository::FileIterator::Equals(const unique_ptr<IteratorInterface> it) const
{
	auto ptr = dynamic_cast<FileRepository::FileIterator*>(it.get());
	if (ptr == nullptr)
		return false;
	return this->currentTower != it->getTower();
}

bool FileRepository::FileIterator::valid() const
{
	return this->currentTower != Tower();
}

void FileRepository::FileIterator::next()
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
		if (temp == this->currentTower)
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
		this->currentTower = Tower();
	}
	else
		this->currentTower = temp;
}
