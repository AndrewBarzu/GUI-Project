#include "repo_in_html.h"
#include "domain.h"
#include <algorithm>
#include <exception>
#include <typeinfo>
#include "setDebugNew.h"
#define new DEBUG_NEW

/// Adds a new tower to the repo
/// Returns a 1 if the location is not unique (not a success), 0 otherwise

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

HTMLRepo::HTMLRepo(std::string filename): FileRepository(filename), first(*this), last(*this)
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
		for (auto iter = this->begin(); iter.valid(); iter++)
		{
			this->mySize += 1;
		}
}

void HTMLRepo::add(const Tower& tower)
{
	std::ifstream is;
	is.open(this->filename, std::ios_base::in);
	Tower readTower;
	std::string line;
	std::vector<Tower> towers;
	for (int i = 0; i < 14; i++)
	{
		std::getline(is, line);
	}
	while (!is.eof())
	{
		std::getline(is, line);
		if (line == "<tr>")
		{
			readTower = read_tower(is);
			if (readTower.get_location() == tower.get_location())
				throw std::exception("Element already exists!");
			towers.push_back(readTower);
		}
		else break;
	}
	is.close();
	towers.push_back(tower);
	this->write_to_file(towers);
	
	this->mySize++;
}

/// Removes a tower from the repo
/// Returns a 1 if the location does not exist (not a success), 0 otherwise
void HTMLRepo::remove(const std::string& location)
{
	std::ifstream is;
	std::fstream fs;
	fs.open(this->filename, std::ios::out | std::ios::app);
	fs.close();
	is.open(this->filename, std::ios_base::in);
	std::vector<Tower> temp;
	std::string line;
	Tower tower;
	for (int i = 0; i < 14; i++)
	{
		std::getline(is, line);
	}
	while (!is.eof())
	{
		std::getline(is, line);
		if (line == "<tr>")
		{
			tower = read_tower(is);
			temp.push_back(tower);
		}
		else break;
	}
	is.close();
	int size;
	size = temp.size();
	temp.erase(std::remove_if(temp.begin(), temp.end(), [location](const Tower& tower) { return tower.get_location() == location; }), temp.end());
	if (temp.size() == size)
		throw std::exception("Tried to remove a non existing element!");
	this->write_to_file(temp);
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
	bool flag = true;
	std::ifstream is;
	std::fstream fs;
	fs.open(this->filename, std::ios::out | std::ios::app);
	fs.close();
	is.open(this->filename, std::ios_base::in);
	std::vector<Tower> temp;
	std::string line;
	Tower readTower;
	for (int i = 0; i < 14; i++)
	{
		std::getline(is, line);
	}
	while (!is.eof())
	{
		std::getline(is, line);
		if (line == "<tr>")
		{
			readTower = read_tower(is);
			temp.push_back(readTower);
		}
		else break;
	}
	is.close();

	auto it = std::find_if(temp.begin(), temp.end(), [tower](const Tower& otherTower) {return otherTower.get_location() == tower.get_location(); });
	if (it == temp.end())
		throw std::exception("Tried to update a non existing element!");

	*it = tower;
	this->write_to_file(temp);
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
int HTMLRepo::size() const
{
	return this->mySize;
}

typename HTMLRepo::iterator& HTMLRepo::begin()
{
	// TODO: insert return statement here
	//this->first = iterator(elements.begin(), *this);
	this->first = iterator(this->filename, *this);
	return this->first;
}

typename HTMLRepo::iterator& HTMLRepo::end()
{
	// TODO: insert return statement here
	//this->last = iterator(elements.end(), *this);
	this->last = iterator(this->filename, *this, "last");
	return this->last;
}


HTMLRepo::iterator::iterator(std::string filename, HTMLRepo& container,std::string pos): repo{ container }
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
			readTower = this->repo.read_tower(is);
			this->current_tower = readTower;
		}
		else
		{
			this->current_tower = Tower();
		}
		is.close();
	}
	else
	{
		this->current_tower = Tower();
	}
}

const Tower& HTMLRepo::iterator::operator*() const
{
	// TODO: insert return statement here
	return this->current_tower;
}

bool HTMLRepo::iterator::operator!=(const RepoInterface::iterator& it) const
{
	HTMLRepo::iterator& iter = dynamic_cast<HTMLRepo::iterator&>(const_cast<RepoInterface::iterator&>(it));
	return this->current_tower != iter.current_tower;
}

bool HTMLRepo::iterator::operator!=(const iterator it) const
{
	return this->current_tower != it.current_tower;
}

bool HTMLRepo::iterator::valid() const
{
	return this->current_tower != Tower();
}

typename HTMLRepo::iterator& HTMLRepo::iterator::operator++()
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
		temp = this->repo.read_tower(is);
		if (temp == this->current_tower)
		{
			std::getline(is, line);
			if (line == "<tr>")
			{
				temp = this->repo.read_tower(is);
			}
			else
				temp = Tower();
			break;
		}
	}
	is.close();

	this->current_tower = temp;
	return *this;
}

typename HTMLRepo::iterator& HTMLRepo::iterator::operator++(int c)
{
	// TODO: insert return statement here
	auto aux = *this;

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
		temp = this->repo.read_tower(is);
		if (temp == this->current_tower)
		{
			std::getline(is, line);
			if (line == "<tr>")
			{
				temp = this->repo.read_tower(is);
			}
			else
				temp = Tower();
			break;
		}
	}
	is.close();

	this->current_tower = temp;
	return aux;
}

HTMLRepo::iterator& HTMLRepo::iterator::operator=(const RepoInterface::iterator& it) {
	HTMLRepo::iterator& iter = dynamic_cast<HTMLRepo::iterator&>(const_cast<RepoInterface::iterator&>(it));
	this->current_tower = iter.current_tower;
	this->filename = iter.filename;
	return *this;
}

HTMLRepo::iterator& HTMLRepo::iterator::operator=(const iterator& it)
{
	this->filename = it.filename;
	this->current_tower = it.current_tower;
	return *this;
}
