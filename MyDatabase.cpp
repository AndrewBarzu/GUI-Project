#include "MyDatabase.h"
#include <winsqlite/winsqlite3.h>
#include "setDebugNew.h"
#define new DEBUG_NEW

MyDatabase::iterator::iterator()
{
	this->currentTower = Tower();
	this->db = NULL;
	this->current = NULL;
}

MyDatabase::iterator::iterator(MyDatabase* db, std::string pos)
{
	this->db = db;
	int rc = NULL;

	std::string sqls = "SELECT * FROM MyTable";
	int err = sqlite3_prepare(this->db->db, sqls.c_str(), -1, &this->current, 0);
	if (err == SQLITE_ERROR)
	{
		throw DBException(sqlite3_errmsg(this->db->db));
	}
	if (pos == "first")
	{
		rc = sqlite3_step(this->current);
		if (rc != SQLITE_DONE)
		{
			std::vector<std::string> params;
			for (int i = 0; i < sqlite3_column_count(this->current); i++)
			{
				std::string text = reinterpret_cast<const char*>(sqlite3_column_text(this->current, i));
				params.push_back(text);
			}
			this->currentTower = Tower(params);
		}
		else
		{
			this->currentTower = Tower();
			sqlite3_finalize(this->current);
			this->current = NULL;
		}
	}
	else
	{
		while (rc != SQLITE_DONE)
			rc = sqlite3_step(this->current);
		this->currentTower = Tower();
		sqlite3_finalize(this->current);
		this->current = NULL;
	}
}

MyDatabase::iterator::iterator(const MyDatabase::iterator& it)
{
	this->db = it.db;
	std::string sqls = "SELECT * FROM MyTable";
	int err = sqlite3_prepare(this->db->db, sqls.c_str(), -1, &this->current, 0);
	if (err == SQLITE_ERROR)
	{
		throw DBException(sqlite3_errmsg(this->db->db));
	}
	while (this->currentTower != it.currentTower)
	{
		int rc = sqlite3_step(this->current);
		if (rc != SQLITE_DONE)
		{
			std::vector<std::string> params;
			for (int i = 0; i < sqlite3_column_count(this->current); i++)
			{
				std::string text = reinterpret_cast<const char*>(sqlite3_column_text(this->current, i));
				params.push_back(text);
			}
			this->currentTower = Tower(params);
		}
		else
		{
			this->currentTower = Tower();
			sqlite3_finalize(this->current);
			this->current = NULL;
		}
	}
}

MyDatabase::iterator::~iterator()
{
	sqlite3_finalize(this->current);
}

const Tower& MyDatabase::iterator::operator*() const
{
	return this->currentTower;
}

bool MyDatabase::iterator::operator!=(const iterator& other) const
{
	if (this->db != other.db)
		throw DBException("Bad comparison! Cannot compare iterators on different databases!");
	return this->currentTower != other.currentTower;
}

bool MyDatabase::iterator::valid() const
{
	return this->currentTower != Tower();
}

MyDatabase::iterator& MyDatabase::iterator::operator=(const MyDatabase::iterator& other)
{
	if (this->current != NULL)
	{
		sqlite3_finalize(this->current);
		this->currentTower = Tower();
	}
	this->db = other.db;
	std::string sqls = "SELECT * FROM MyTable";
	int err = sqlite3_prepare(this->db->db, sqls.c_str(), -1, &this->current, 0);
	if (err == SQLITE_ERROR)
	{
		throw DBException(sqlite3_errmsg(this->db->db));
	}
	while (this->currentTower != other.currentTower)
	{
		int rc = sqlite3_step(this->current);
		if (rc != SQLITE_DONE)
		{
			std::vector<std::string> params;
			for (int i = 0; i < sqlite3_column_count(this->current); i++)
			{
				std::string text = reinterpret_cast<const char*>(sqlite3_column_text(this->current, i));
				params.push_back(text);
			}
			this->currentTower = Tower(params);
		}
		else
		{
			this->currentTower = Tower();
			sqlite3_finalize(this->current);
			this->current = NULL;
		}
	}
	return *this;
}

MyDatabase::iterator MyDatabase::iterator::operator++()
{
	if (!this->valid())
		throw DBException("Invalid preincrement!");
	int rc = sqlite3_step(this->current);
	if (rc != SQLITE_DONE)
	{
		std::vector<std::string> params;
		for (int i = 0; i < sqlite3_column_count(this->current); i++)
		{
			std::string text = reinterpret_cast<const char*>(sqlite3_column_text(this->current, i));
			params.push_back(text);
		}
		this->currentTower = Tower(params);
	}
	else
	{
		this->currentTower = Tower();
		sqlite3_finalize(this->current);
		this->current = NULL;
	}
	return *this;
}

MyDatabase::iterator MyDatabase::iterator::operator++(int c)
{
	auto aux = *this;
	if (!this->valid())
		throw DBException("Invalid preincrement!");
	int rc = sqlite3_step(this->current);
	if (rc != SQLITE_DONE)
	{
		std::vector<std::string> params;
		for (int i = 0; i < sqlite3_column_count(this->current); i++)
		{
			std::string text = reinterpret_cast<const char*>(sqlite3_column_text(this->current, i));
			params.push_back(text);
		}
		this->currentTower = Tower(params);
	}
	else
	{
		this->currentTower = Tower();
		sqlite3_finalize(this->current);
		this->current = NULL;
	}

	return aux;
}

static int _stdcall callback(void* data, int argc, char** argv, char** azColName) {
	int i;
	std::vector<std::string> params;
	for (i = 0; i < argc; i++) {
		params.push_back(argv[i]);
	}
	Tower aux{ params };
	((std::vector<Tower>*)data)->push_back(aux);
	return 0;
}

static int _stdcall sizeCallback(void* data, int argc, char** argv, char** azColName)
{
	std::string res{ argv[0] };
	*((int*)data) = std::stoi(res);
	return 0;
}

MyDatabase::MyDatabase(const std::string& filename)
{
	this->filename = filename;
	int rc = sqlite3_open(filename.c_str(), &this->db);
	char* error;
	if (rc)
		throw DBException("Error opening database!");
	const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS MyTable (\
location TEXT PRIMARY KEY, \
size TEXT NOT NULL, \
auraLevel TEXT NOT NULL, \
parts TEXT NOT NULL, \
vision TEXT NOT NULL);";
	rc = sqlite3_exec(this->db, sqlCreateTable, NULL, NULL, &error);
	if (rc)
	{
		std::string err{ error };
		sqlite3_free(error);
		throw DBException(err);
	}
}

void MyDatabase::add(const Tower& tower)
{
	int rc;
	char* error;

	std::string query = "INSERT INTO MyTable (location, size, auraLevel, parts, vision) VALUES \
		('" + tower.get_location() + "', '" + tower.get_size() + "', '" + tower.get_aura_level() + "', '" + tower.get_parts() + "', '" + tower.get_vision() + "');";
	rc = sqlite3_exec(this->db, query.c_str(), NULL, NULL, &error);
	if (rc)
	{
		std::string err{ error };
		sqlite3_free(error);
		throw DBException(err);
	}
}

void MyDatabase::remove(const std::string& location)
{

	std::string query = "DELETE FROM MyTable WHERE location='" + location + "';";
	char* error;
	int rc = sqlite3_exec(this->db, query.c_str(), NULL, NULL, &error);

	if (rc)
	{
		std::string err{ error };
		sqlite3_free(error);
		throw DBException(err);
	}

	if (!sqlite3_changes(this->db))
		throw DBException("No element was removed!");
}

void MyDatabase::update(const Tower& tower)
{

	std::string query = "UPDATE MyTable \
SET \
size='" + tower.get_size() + "', \
auraLevel='" + tower.get_aura_level() + "', \
parts='" + tower.get_parts() + "', \
vision='" + tower.get_vision() + "' \
WHERE location='" + tower.get_location() + "';";
	char* error;
	int rc = sqlite3_exec(this->db, query.c_str(), NULL, NULL, &error);
	if (rc)
	{
		std::string err{ error };
		sqlite3_free(error);
		throw DBException(err);
	}

	if (!sqlite3_changes(this->db))
		throw DBException("No update has been made!");


}

Tower MyDatabase::search(std::string location)
{

	std::string query = "SELECT * FROM MyTable WHERE location='" + location + "';";
	char* error;
	std::vector<Tower> result;
	int rc = sqlite3_exec(this->db, query.c_str(), callback, &result, &error);
	if (rc)
	{
		std::string err{ error };
		sqlite3_free(error);
		throw DBException(err);
	}

	return result[0];
}

std::vector<Tower> MyDatabase::get_all()
{

	std::string query = "SELECT * FROM MyTable;";
	char* error;
	std::vector<Tower> result;
	int rc = sqlite3_exec(this->db, query.c_str(), callback, &result, &error);
	if (rc)
	{
		std::string err{ error };
		sqlite3_free(error);
		throw DBException(err);
	}
	return result;
}

MyDatabase::iterator MyDatabase::begin()
{
	return MyDatabase::iterator(this, "first");
}

MyDatabase::iterator MyDatabase::end()
{
	return MyDatabase::iterator(this, "last");
}

int MyDatabase::size()
{

	std::string command = "SELECT COUNT() from MyTable";
	char* error;
	int size;
	int rc = sqlite3_exec(this->db, command.c_str(), sizeCallback, &size, &error);
	if (rc)
	{
		std::string err{ error };
		sqlite3_free(error);
		throw DBException(err);
	}

	return size;
}

MyDatabase::~MyDatabase()
{
	sqlite3_close(this->db);
}