#include "repository_in_file.h"
#include "domain.h"
#include <algorithm>
#include <exception>
#include <typeinfo>
#include "setDebugNew.h"
#define new DEBUG_NEW

using namespace std;

std::vector<std::string> split(const std::string& input, char delimiter) {
	std::vector<std::string> result;
	std::stringstream input_stream(input);
	std::string token;
	while (getline(input_stream, token, delimiter))
		result.push_back(token);

	return result;
}


void FileRepository::write_to_file()
{
	std::ofstream os;
	os.open(this->filename, std::ios_base::out);
	for (const auto& tower : this->elements)
	{
		os << tower << '\n';
	}
	os.close();
}

void FileRepository::read_from_file()
{
	fstream is;
	is.open(this->filename, std::ios::in);
	while (is.good())
	{
		string line;
		getline(is, line);
		if (line != "")
			this->elements.push_back(Tower(split(line, ',')));
	}
	is.close();
}

/// Adds a new tower to the repo
/// Returns a 1 if the location is not unique (not a success), 0 otherwise
void FileRepository::add(const Tower& tower)
{
	auto exists = find(this->elements.begin(), this->elements.end(), tower);
	if (exists != this->elements.end())
		throw exception("Element not unique!");
	std::ofstream os;
	os.open(this->filename, std::ios_base::app);
	os << tower << '\n';
	os.close();
	this->elements.push_back(tower);
	this->mySize++;
}

/// Removes a tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void FileRepository::remove(const std::string& location)
{
	int size;
	size = this->elements.size();
	this->elements.erase(std::remove_if(this->elements.begin(), this->elements.end(), [location](const Tower& tower) { return tower.get_location() == location; }), this->elements.end());
	if (this->elements.size() == size)
		throw std::exception("Tried to remove a non existing element!");
	this->write_to_file();
	this->mySize--;
}

/// Updates a new tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void FileRepository::update(const Tower& tower)
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	if (it == this->elements.end())
		throw std::exception("Tried to update a non existing element!");

	*it = tower;
	this->write_to_file();
}

Tower FileRepository::search(const std::string& location) const
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [location](const Tower& mytower) {return mytower.get_location() == location; });
	if (it == this->elements.end())
		return Tower();
	return *it;
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
	return make_unique<FileIterator>(this);
}

unique_ptr<RepoInterface::IteratorInterface> FileRepository::end() const
{
	// TODO: insert return statement here
	//this->last = iterator(elements.end(), *this);
	return make_unique<FileIterator>(this, "last");
}


FileRepository::FileIterator::FileIterator(const FileRepository* container, std::string pos): 
	IteratorInterface{ container }
{
	if (pos == "first" && container->elements.size() > 0)
	{
		this->currentPos = 0;
	}
	else
	{
		this->currentPos = -1;
	}
}

FileRepository::FileIterator::FileIterator(const FileIterator& other): IteratorInterface{other.container}
{
	this->currentTower = other.currentTower;
	this->currentPos = other.currentPos;
}

void FileRepository::FileIterator::first()
{
	if (((FileRepository*)this->container)->elements.size() > 0)
		this->currentPos = 0;
	else
		this->currentPos = -1;
}

const Tower& FileRepository::FileIterator::getTower() const
{
	// TODO: insert return statement here
	if (this->currentPos != -1)
		return ((FileRepository*)this->container)->elements[currentPos];
	return Tower();
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
	return this->currentPos != -1;
}

void FileRepository::FileIterator::next()
{
	// TODO: insert return statement here
	if (this->currentPos != -1 && this->currentPos + 1 < ((FileRepository*)this->container)->elements.size())
		this->currentPos++;
	else
		this->currentPos = -1;
}
