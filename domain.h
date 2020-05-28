#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class Tower
{
private:
	std::string auraLevel;
	std::string parts;
	std::string size;
	std::string location;
	std::string vision;
	std::string printable;
	friend std::ostream& operator<<(std::ostream&, const Tower&);
	friend std::istream& operator>>(std::istream&, Tower&);
public:
	Tower() 
	{
		auraLevel = "";
		parts = "";
		size = "";
		location = "";
		vision = "";
		printable = "";
	}
	Tower(const std::vector<std::string>& params);
	Tower(const Tower&);
	virtual Tower& operator=(const Tower&);
	virtual std::string get_aura_level() const;
	virtual std::string get_parts() const;
	virtual std::string get_size() const;
	virtual std::string get_location() const;
	virtual std::string get_vision() const;
	virtual std::string print() const;
	virtual bool operator!=(const Tower&) const;
	virtual bool operator==(const Tower&) const;
	virtual ~Tower();
};
