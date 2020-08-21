#include "repo_in_html.h"
#include "domain.h"
#include <algorithm>
#include <exception>
#include <typeinfo>
#include "setDebugNew.h"
#define new DEBUG_NEW

using namespace std;


Tower HTMLRepo::read_tower(std::istream& is)
{
	std::vector<std::string> params(5);
	std::string extra;
	std::getline(is, params[0]);
	std::getline(is, params[1]);
	std::getline(is, params[2]);
	std::getline(is, params[3]);
	std::getline(is, params[4]);
	/*std::getline(is, params[3]);
	std::getline(is, params[2]);
	std::getline(is, params[0]);
	std::getline(is, params[1]);
	std::getline(is, params[4]);*/
	params[0].replace(0, 4, "");
	params[0].replace(params[0].size() - 5, params[0].size(), "");
	params[1].replace(0, 4, "");
	params[1].replace(params[1].size() - 5, params[1].size(), "");
	params[2].replace(0, 4, "");
	params[2].replace(params[2].size() - 5, params[2].size(), "");
	params[3].replace(0, 4, "");
	params[3].replace(params[3].size() - 5, params[3].size(), "");
	params[4].replace(0, 4, "");
	params[4].replace(params[4].size() - 5, params[4].size(), "");
	std::getline(is, extra);
	return Tower(params);
}

void HTMLRepo::write_tower(std::ostream& os, const Tower& tower)
{
	os << "<tr>\n";
	os << "<td>" << tower.get_location() << "</td>\n";
	os << "<td>" << tower.get_size() << "</td>\n";
	os << "<td>" << tower.get_aura_level() << "</td>\n";
	os << "<td>" << tower.get_parts() << "</td>\n";
	os << "<td>" << tower.get_vision() << "</td>\n";
	os << "</tr>\n";
}

void HTMLRepo::write_to_file(const std::vector<Tower>& towers)
{
	std::fstream fs;
	fs.open(this->filename, std::ios::out);
	fs << "<!DOCTYPE html>\n";
	fs << "<html>\n\
<head>\n\
<title>Towers</title>\n\
</head>\n\<body>\n\
<table border = \"1\">\n\
<tr>\n\
<td>Location</td>\n\
<td>Size</td>\n\
<td>Aura level</td>\n\
<td>Parts</td>\n\
<td>Vision</td>\n\
</tr>\n";

	for (const auto& tower : towers)
	{
		write_tower(fs, tower);
	}

	fs << "</table>\n\
</body>\n\
</html>\n";
	fs.close();
}

HTMLRepo::HTMLRepo(std::string filename): FileRepository(filename)
{
	this->mySize = 0;
	std::fstream is;
	is.open(this->filename, std::ios::in);
	if (is.peek() == std::ofstream::traits_type::eof())
	{
		is.close();
		is.open(this->filename, std::ios::out);
		is << "<!DOCTYPE html>\n";
		is << "<html>\n\
				<head>\n\
				<title>Towers</title>\n\
				</head>\n\
				<body>\n\
				<table border = \"1\">\n\
				<tr>\n\
				<td>Location</td>\n\
				<td>Size</td>\n\
				<td>Aura level</td>\n\
				<td>Parts</td>\n\
				<td>Vision</td>\n\
				</tr>\n\
				</table>\n\
				</body>\n\
				</html>\n";
		is.close();
	}
	else
		for (auto iter = this->begin(); iter->valid(); iter->next())
		{
			this->elements.push_back(iter->getTower());
			this->mySize += 1;
		}
}

void HTMLRepo::add(const Tower& tower)
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	if (it != this->elements.end())
		throw std::exception("Element already exists!");
	this->elements.push_back(tower);
	this->write_to_file(this->elements);
	this->mySize++;
}

/// Removes a tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void HTMLRepo::remove(const std::string& location)
{
	int oldSize = this->elements.size();
	this->elements.erase(std::remove_if(this->elements.begin(), this->elements.end(), [location](const Tower& tower) { return tower.get_location() == location; }), this->elements.end());
	if (this->elements.size() == oldSize)
		throw std::exception("Tried to remove a non existing element!");
	this->write_to_file(this->elements);
	this->mySize--;
	/*bool flag = true;
	int oldSize = this->elements.size();
	this->elements.erase(std::remove_if(this->elements.begin(), this->elements.end(), [params](const Tower& tower) { return tower.get_location() == params[0]; }), this->elements.end());
	if (this->elements.size() == oldSize)
		return 1;*/
}

/// Updates a new tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void HTMLRepo::update(const Tower& tower)
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [tower](const Tower& mytower) {return mytower.get_location() == tower.get_location(); });
	if (it != this->elements.end())
	{
		*it = tower;
		this->write_to_file(this->elements);
		return;
	}
	throw std::exception("Tried to update a non existing element!");
}

Tower HTMLRepo::search(const std::string& location) const
{
	auto it = std::find_if(this->elements.begin(), this->elements.end(), [location](const Tower& mytower) {return mytower.get_location() == location; });
	if (it != this->elements.end())
		return *it;
	return Tower();
}

unique_ptr<RepoInterface::IteratorInterface> HTMLRepo::begin() const
{
	// TODO: insert return statement here
	//this->first = iterator(elements.begin(), *this);
	return make_unique<HTMLIterator>(this, this->filename);
}

unique_ptr<RepoInterface::IteratorInterface> HTMLRepo::end() const
{
	// TODO: insert return statement here
	//this->last = iterator(elements.end(), *this);
	return make_unique<HTMLIterator>(this, this->filename, "last");
}


HTMLRepo::HTMLIterator::HTMLIterator(const HTMLRepo* container, std::string filename, std::string pos): IteratorInterface{ container }
{
	this->filename = filename;
	if (pos == "first")
	{

		std::ifstream is;
		std::string line;
		Tower readTower;
		is.open(filename);
		for (int i = 0; i < 14; i++)
		{
			std::getline(is, line);
		}
		std::getline(is, line);
		if (line == "<tr>")
		{
			readTower = ((HTMLRepo*)this->container)->read_tower(is);
			this->currentTower = readTower;
		}
		else
		{
			this->currentTower = Tower();
		}
		is.close();
	}
	else
	{
		this->currentTower = Tower();
	}
}

//HTMLRepo::HTMLIterator::HTMLIterator(const unique_ptr<IteratorInterface> other): IteratorInterface{other->container}
//{
//	this->currentTower = other.currentTower;
//	this->filename = other.filename;
//}

void HTMLRepo::HTMLIterator::first()
{
	std::ifstream is;
	std::string line;
	Tower readTower;
	is.open(filename);
	for (int i = 0; i < 14; i++)
	{
		std::getline(is, line);
	}
	std::getline(is, line);
	if (line == "<tr>")
	{
		readTower = ((HTMLRepo*)this->container)->read_tower(is);
		this->currentTower = readTower;
	}
	else
	{
		this->currentTower = Tower();
	}
	is.close();
}

const Tower& HTMLRepo::HTMLIterator::getTower() const
{
	// TODO: insert return statement here
	return this->currentTower;
}

bool HTMLRepo::HTMLIterator::Equals(const unique_ptr<RepoInterface::IteratorInterface> it) const
{
	auto ptr = dynamic_cast<HTMLRepo::HTMLIterator*>(it.get());
	if (ptr == nullptr)
		return false;
	return this->currentTower == it->getTower();
}

bool HTMLRepo::HTMLIterator::valid() const
{
	return this->currentTower != Tower();
}

void HTMLRepo::HTMLIterator::next()
{
	// TODO: insert return statement here
	std::ifstream is;
	is.open(filename, std::ios_base::in);
	Tower temp;
	bool eof = false;
	std::string line;
	for (int i = 0; i < 14; i++)
	{
		std::getline(is, line);
	}
	while (true)
	{
		std::getline(is, line);
		if (line != "<tr>")
			break;
		temp = ((HTMLRepo*)this->container)->read_tower(is);
		if (temp == this->currentTower)
		{
			std::getline(is, line);
			if (line == "<tr>")
			{
				temp = ((HTMLRepo*)this->container)->read_tower(is);
			}
			else
				temp = Tower();
			break;
		}
	}
	is.close();

	this->currentTower = temp;
}