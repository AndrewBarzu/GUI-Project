#pragma once
#include "domain.h"
#include <cstring>
#include <exception>

class TowerException : public std::exception
{
public:
	TowerException(std::string e = ""): std::exception{ e.c_str() }{}
};

class Helper
{
public:
	static void tower_validator(Tower tower)
	{
		int i;

		try 
		{
			i = std::stoi(tower.get_aura_level().c_str());
		}
		catch (...)
		{
			throw TowerException("Aura level is not an int!");
		}

		if (tower.get_aura_level() != std::to_string(i))
		{
			throw TowerException("Aura level is not an int!");
		}


		try 
		{
			i = std::stoi(tower.get_parts().c_str());
		}
		catch (...)
		{
			throw TowerException("Parts is not an int!");
		}

		if (tower.get_parts() != std::to_string(i))
		{
			throw TowerException("Parts is not an int!");
		}
	}
};