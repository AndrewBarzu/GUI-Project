#include "domain.h"
#include <cstdlib>
#include <vector>
#include <string>
#include "setDebugNew.h"
#define new DEBUG_NEW

Tower::Tower(const std::vector<std::string>& params)
{
	location = params[0];
	size = params[1];
	auraLevel = params[2];
	parts = params[3];
	vision = params[4];
	printable = location + "," + size + "," + auraLevel + "," + parts + "," + vision;//new char[strlen(params[0]) + 2 + strlen(params[1]) + 2 + strlen(params[2]) + 2 + strlen(params[3]) + 2 + strlen(params[4]) + 2];
	//printable = auraLevel + ", " + parts + ", " + size + ", " + location + ", " + vision;
}

Tower::Tower(const Tower& tower)
{
	auraLevel = tower.auraLevel;
	parts = tower.parts;
	size = tower.size;
	location = tower.location;
	vision = tower.vision;
	printable = location + "," + size + "," + auraLevel + "," + parts + "," + vision;
	//printable = auraLevel + ", " + parts + ", " + size + ", " + location + ", " + vision;
}

Tower& Tower::operator=(const Tower& other)
{
	this->auraLevel = other.auraLevel;
	this->parts = other.parts;
	this->size = other.size;
	this->location = other.location;
	this->vision = other.vision;
	this->printable = other.printable;
	// TODO: insert return statement here
	return *this;
}

//Tower& Tower::operator=(Tower& tower)
//{
//	if (this == &tower)
//		return *this;
//
//	this->~Tower();
//
//	auraLevel = new char[strlen(tower.auraLevel) + 1];
//	strcpy(auraLevel, tower.auraLevel);
//	parts = new char[strlen(tower.parts) + 1];
//	strcpy(parts, tower.parts);
//	size = new char[strlen(tower.size) + 1];
//	strcpy(size, tower.size);
//	location = new char[strlen(tower.location) + 1];
//	strcpy(location, tower.location);
//	vision = new char[strlen(tower.vision) + 1];
//	strcpy(vision, tower.vision);
//	printable = new char[strlen(tower.auraLevel) + 2 + strlen(tower.parts) + 2 + strlen(tower.location) + 2 + strlen(tower.size) + 2 + strlen(tower.vision) + 2];
//	printable[0] = 0;
//
//	strcat(printable, tower.location);
//	strcat(printable, ", ");
//	strcat(printable, tower.size);
//	strcat(printable, ", ");
//	strcat(printable, tower.auraLevel);
//	strcat(printable, ", ");
//	strcat(printable, tower.parts);
//	strcat(printable, ", ");
//	strcat(printable, tower.vision);
//	strcat(printable, "\0");
//
//	active = tower.active;
//
//	return *this;
//}

//Tower Tower::operator=(Tower tower)
//{
//	auraLevel = new char[strlen(tower.auraLevel) + 1];
//	strcpy(auraLevel, tower.auraLevel);
//	parts = new char[strlen(tower.parts) + 1];
//	strcpy(parts, tower.parts);
//	size = new char[strlen(tower.size) + 1];
//	strcpy(size, tower.size);
//	location = new char[strlen(tower.location) + 1];
//	strcpy(location, tower.location);
//	vision = new char[strlen(tower.vision) + 1];
//	strcpy(vision, tower.vision);
//	printable = new char[strlen(tower.auraLevel) + 2 + strlen(tower.parts) + 2 + strlen(tower.location) + 2 + strlen(tower.size) + 2 + strlen(tower.vision) + 2];
//	printable[0] = 0;
//	strcat(printable, tower.location);
//	strcat(printable, ", ");
//	strcat(printable, tower.size);
//	strcat(printable, ", ");
//	strcat(printable, tower.auraLevel);
//	strcat(printable, ", ");
//	strcat(printable, tower.parts);
//	strcat(printable, ", ");
//	strcat(printable, tower.vision);
//	strcat(printable, "\0");
//	return *this;
//}

std::string Tower::get_aura_level() const
{
	return auraLevel;
}

std::string Tower::get_parts() const
{
	return parts;
}

std::string Tower::get_size() const
{
	return size;
}

std::string Tower::get_location() const
{
	return location;
}

std::string Tower::get_vision() const
{
	return vision;
}

std::string Tower::print() const
{
	return printable;
}

bool Tower::operator!=(const Tower& other) const
{
	return this->location != other.location;
}

bool Tower::operator==(const Tower& other) const
{
	return this->location == other.location;
}

Tower::~Tower()
{

}

std::ostream& operator<<(std::ostream& os, const Tower& tower)
{
	// TODO: insert return statement here
	os << tower.printable;
	return os;
}

std::istream& operator>>(std::istream& is, Tower& tower)
{
	// TODO: insert return statement here
	std::string str;
	std::getline(is, str);
	/*
	std::istringstream iss{ str };
	std::string token;
	str = "";
	while (std::getline(iss, token, ','))
	{
		str += token;
	}
	iss.str(str);*/
	std::istringstream iss{ str };
	std::getline(iss, tower.location, ',');
	std::getline(iss, tower.size, ',');
	std::getline(iss, tower.auraLevel, ',');
	std::getline(iss, tower.parts, ',');
	std::getline(iss, tower.vision, ',');
	tower.printable = tower.location + "," + tower.size + "," + tower.auraLevel + "," + tower.parts + "," + tower.vision;
	return is;
}
