#include <vector>
#include "domain.h"
#include "repository_in_memory.h"
#include "repository.h"
#include "controller.h"
#include "repository_in_file.h"
#include "repo_in_html.h"
#include "DBRepository.h"
#include "UI.h"
#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <Windows.h>
#include "GUI.h"
#include <QtWidgets/QApplication>
#include <regex>
#include "reportingHook.h"
#include "setDebugNew.h"

#define new DEBUG_NEW

class MyDialogBox : public QDialog
{
private:
	QPushButton* OKButton;

	QLineEdit* path;
public:
	MyDialogBox(std::string errorText, std::string pathStrings)
	{
		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setAlignment(Qt::AlignTop);

		QLabel* errorLabel;
		if (errorText != "")
		{
			errorLabel = new QLabel(errorText.c_str());

		}

		QFormLayout* form = new QFormLayout(); 

		this->path = new QLineEdit();

		QLabel* pathLabel = new QLabel(QString::fromStdString("&" + pathStrings));
		pathLabel->setBuddy(this->path);

		form->addRow(pathLabel, this->path);

		layout->addLayout(form);
		this->OKButton = new QPushButton();
		this->OKButton->setText("OK");
		this->OKButton->setMaximumSize(100, 30);
		QObject::connect(this->OKButton, &QPushButton::clicked, this, &MyDialogBox::clickHandler);

		layout->addWidget(OKButton);

		//QObject::connect(this->OKButton, &QPushButton::clicked, this, initPath)
	}
	std::string getString()
	{
		return this->path->text().toStdString();
	}
private:
	void clickHandler()
	{
		if (this->path->text() != "")
		{
			emit QDialog::accept();
		}
	}
	virtual void closeEvent(QCloseEvent* e) override
	{
		emit QDialog::reject();
	}
};

void test_domain()
{
	std::vector<std::string> params;
	params.push_back("wherever");
	params.push_back("big boi");
	params.push_back("10");
	params.push_back("100");
	params.push_back("got that");
	Tower tower = Tower(params);
	Tower* tower2 = &tower;
	*tower2 = tower;
	assert(tower.get_aura_level() == "10");
	assert(tower.get_location() == "wherever");
	assert(tower.get_parts() == "100");
	assert(tower.get_size() == "big boi");
	assert(tower.get_vision() == "got that");
	//assert(tower.print() == "10, 100, big boi, wherever, got that");
	assert(tower.print() == "wherever,big boi,10,100,got that");
	//std::cin >> tower;
	//std::cout << tower;
}

void test_repo()
{
	std::vector<std::string> params;
	params.push_back("wherever");
	params.push_back("big boi");
	params.push_back("10");
	params.push_back("100");
	params.push_back("got that");
	RepoInMemory repository = RepoInMemory();
	Tower tower(params);
	repository.add(tower);
	try
	{
		repository.add(tower);
		assert(false);
	}
	catch (std::exception e)
	{
		assert(true);
	}

	assert(repository.size() == 1);
	params[0] = "new location";
	tower = Tower(params);
	repository.add(tower);
	auto iter = repository.begin();
	auto last = repository.end();
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	iter->first();
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	repository.update(Tower(params));
	repository.remove("new location");
	try
	{
		repository.remove("new location");
		assert(false);
	}
	catch (std::exception e)
	{
		assert(true);
	}
	repository.remove("wherever");
	try
	{
		repository.update(tower);
		assert(false);
	}
	catch (std::exception)
	{
		assert(true);
	}
	assert(repository.size() == 0);
}

void test_file_repo()
{
	std::vector<std::string> params;
	params.push_back("wherever");
	params.push_back("big boi");
	params.push_back("10");
	params.push_back("100");
	params.push_back("got that");
	FileRepository repository = FileRepository("myfile.txt");
	Tower tower(params);
	repository.add(tower);
	try
	{
		repository.add(tower);
		assert(false);
	}
	catch (std::exception e)
	{
		assert(true);
	}

	assert(repository.size() == 1);
	params[0] = "new location";
	tower = Tower(params);
	repository.add(tower);
	auto iter = repository.begin();
	auto last = repository.end();
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	iter->first();
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	repository.update(Tower(params));
	repository.remove("new location");
	try
	{
		repository.remove("new location");
		assert(false);
	}
	catch (std::exception e)
	{
		assert(true);
	}
	repository.remove("wherever");
	try
	{
		repository.update(tower);
		assert(false);
	}
	catch (std::exception)
	{
		assert(true);
	}
	assert(repository.size() == 0);
	remove("myfile.txt");
}

void test_controller()
{
	RepoInMemory repo = RepoInMemory();
	FileRepository list = FileRepository("myList.csv");
	Controller controller(&repo, &list);
	char location[10] = "here";
	char size[10] = "big boi";
	char levelTest[10] = "10";
	char partsTest[10] = "100";
	char vision[10] = "got that";

	controller.add(location, size, levelTest, partsTest, vision);
	controller.add("whenever", "big", "100", "1000", "bigg");
	controller.add("wherever", "big", "100", "1000", "bigg");
	controller.add("we're", "big", "100", "1000", "bigg");
	controller.add("meant", "big", "100", "1000", "bigg");
	controller.add("to", "big", "100", "1000", "bigg");
	controller.add("be", "big", "100", "1000", "bigg");
	controller.add("together", "big", "100", "1000", "bigg");
	controller.change_mode("B");
	try
	{
		controller.add(location, size, levelTest, partsTest, vision);
		assert(false);
	}
	catch (const std::exception& e)
	{
		assert(true);
	}
	controller.change_mode("A");
	controller.update(location, "big boi", "100", "1000", "big");
	controller.change_mode("B");
	try
	{
		controller.update(location, "11", "100", "1000", "big");
		assert(false);
	}
	catch (const std::exception& e)
	{
		assert(true);
	}
	controller.change_mode("A");
	controller.change_mode("B");
	try
	{
		controller.remove(location);
		assert(false);
	}
	catch (const std::exception& e)
	{
		assert(true);
	}
	controller.list("big");
	controller.next();
	controller.list(size);
	controller.next();
	controller.list("small");
	controller.next();
	controller.list("");
	controller.next();
	controller.change_mode("A");
	controller.remove(location);
	assert(controller.getMode() == "A");
	try
	{
		controller.save(location);
	}
	catch (const std::exception& e)
	{
		assert(true);
	}
	controller.save("together");
	//assert(controller.mylist().size() != 0);
	assert(controller.print().size() != 0);
	/*assert(controller.save("here") == 1);
	assert(controller.save("whenever") == 0);
	for (auto& it : controller.repo_iterator())
	{
		;
	}*/
	assert(controller.change_mode("A") == 1);
	remove("myList.csv");
}

void test_db_repo()
{
	{
		std::vector<std::string> params;
		params.push_back("wherever");
		params.push_back("big boi");
		params.push_back("10");
		params.push_back("100");
		params.push_back("got that");
		DBRepository repository = DBRepository("mydb.db");
		Tower tower(params);
		repository.add(tower);
		try
		{
			repository.add(tower);
			assert(false);
		}
		catch (std::exception e)
		{
			assert(true);
		}

		assert(repository.size() == 1);
		params[0] = "new location";
		tower = Tower(params);
		repository.add(tower);
		auto iter = repository.begin();
		auto last = repository.end();
		for (iter; iter->valid(); iter->next())
		{
			continue;
		}
		for (iter; iter->valid(); iter->next())
		{
			continue;
		}
		iter->first();
		for (iter; iter->valid(); iter->next())
		{
			continue;
		}
		repository.update(Tower(params));
		repository.remove("new location");
		try
		{
			repository.remove("new location");
			assert(false);
		}
		catch (std::exception e)
		{
			assert(true);
		}
		repository.remove("wherever");
		try
		{
			repository.update(tower);
			assert(false);
		}
		catch (std::exception)
		{
			assert(true);
		}
		assert(repository.size() == 0);
	}
	remove("mydb.db");
}

void test_html()
{
	std::vector<std::string> params;
	params.push_back("wherever");
	params.push_back("big boi");
	params.push_back("10");
	params.push_back("100");
	params.push_back("got that");
	HTMLRepo repository = HTMLRepo("myfile.html");
	Tower tower(params);
	repository.add(tower);
	try
	{
		repository.add(tower);
		assert(false);
	}
	catch (std::exception e)
	{
		assert(true);
	}

	assert(repository.size() == 1);
	params[0] = "new location";
	tower = Tower(params);
	repository.add(tower);
	auto iter = repository.begin();
	auto last = repository.end();
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	iter->first();
	for (iter; iter->valid(); iter->next())
	{
		continue;
	}
	repository.update(Tower(params));
	repository.remove("new location");
	try
	{
		repository.remove("new location");
		assert(false);
	}
	catch (std::exception e)
	{
		assert(true);
	}
	repository.remove("wherever");
	try
	{
		repository.update(tower);
		assert(false);
	}
	catch (std::exception)
	{
		assert(true);
	}
	assert(repository.size() == 0);
	remove("myfile.html");
}

void test_database()
{
	{
		MyDatabase db{ "database.db" };
		std::vector<std::string> params;
		params.push_back("wherever");
		params.push_back("big boi");
		params.push_back("10");
		params.push_back("100");
		params.push_back("gay");
		Tower tower = Tower(params);
		db.add(tower);
		Tower querriedTower = db.search("wherever");
		std::cout << querriedTower << "\n\n";
		params[1] = "humongous boi";
		params[2] = "5000";
		params[3] = "one bazzilion";
		params[4] = "chonk";
		tower = Tower(params);
		db.update(tower);
		params[0] = "different place";
		db.add(Tower(params));
		int count = 0;
		for (auto tower : db)
		{
			count++;
		}
		assert(count == 2);
		assert(db.size() == 2);
		db.remove("different place");
		//std::cout << '\n';
		count = 0;
		for (auto tower : db)
		{
			count++;
		}
		assert(count == 1);
		for (auto tower = db.begin(); tower != db.end(); ++tower)
			continue;
		assert(db.size() == 1);
		db.remove("wherever");
		assert(db.size() == 0);
	}
	remove("database.db");
}

DWORD WINAPI startProcess(LPVOID file)
{
	system((char*)file);
	return 0;
}

std::string read_config(RepoInterface** repo, FileRepository** saved)
{
	std::stringstream stream;
	std::string line;
	std::ifstream file;
	file.open("config.txt", std::ios::in);
	std::string choice;
	while (!file.eof())
	{
		std::getline(file, line);
		if (line[0] == '#')
			continue;
		stream.str(line);
		std::string prop, value;
		std::getline(stream, prop, '=');
		std::getline(stream, value);
		stream.clear();

		if (prop == "repository")
		{
			if (value == "inmemory")
			{
				*repo = new RepoInMemory();
				choice = "1";
			}
			else if (value == "dbrepo")
			{
				*repo = new DBRepository("dbrepo.db");
				choice = "2";
			}
			else if (value == "filerepo")
			{
				*repo = new FileRepository("filerepo.txt");
				choice = "3";
			}
			else if (value == "htmlrepo")
			{
				*repo = new HTMLRepo("htmlrepo.html");
				choice = "4";
			}
		}
		else if (prop == "saved_list")
		{
			if (value == "csvlist" || value == "txtlist")
			{
				qDebug() << QString::fromStdString("filelist" + choice + "." + value.substr(0, 3));
				*saved = new FileRepository("filelist" + choice + "." + value.substr(0, 3));
				if (choice == "1")
				{
					choice = "filelist" + choice + "." + value.substr(0, 3);
				}
				else choice = "";
				break;
			}
			else if (value == "htmllist")
			{
				*saved = new HTMLRepo("htmllist" + choice + ".html");
				if (choice == "1")
				{
					choice = "filelist" + choice + "." + value.substr(0, 3);
				}
				else choice = "";
				break;
			}
		}
	}
	return choice;
}

int main(int argc, char* argv[])
{
	//test_vector();
	test_domain();
	test_repo();
	test_controller();
	test_file_repo();
	test_db_repo();
	test_database();
	test_html();
	int ret;
	std::string listpath;
	{
		QApplication a(argc, argv);
		std::string path1, path2, error = "";
		RepoInterface* repo = nullptr;
		FileRepository* list = nullptr;
		listpath = read_config(&repo, &list);
		/*while (true)
		{
			MyDialogBox dialog{error, "Repository path"};
			if (dialog.exec() == QDialog::Accepted) {
				path1 = dialog.getString();
				if (path1 == "inmemory")
				{
					repo = new RepoInMemory();
					break;
				}
				else if (std::regex_match(path1, std::regex(".*\\.db$")))
				{
					repo = new DBRepository(path1);
					break;
				}
				else if (std::regex_match(path1, std::regex(".*\\.txt$")))
				{
					repo = new FileRepository(path1);
					break;
				}
				else if (std::regex_match(path1, std::regex(".*\\.html$")))
				{
					repo = new HTMLRepo(path1);
					break;
				}
				else
				{
					error = "Invalid extension, try again!";
				}
			}
			else goto endOfProgram;
		}
		while (true)
		{
			MyDialogBox dialog{ error , "Saved list path"};
			if (dialog.exec() == QDialog::Accepted) {
				path2 = dialog.getString();
				if (std::regex_match(path2, std::regex(".*\\.csv$")) || std::regex_match(path2, std::regex(".*\\.txt$")))
				{
					list = new FileRepository(path2);
					break;
				}
				else if (std::regex_match(path2, std::regex(".*\\.html$")))
				{
					list = new HTMLRepo(path2);
					break;
				}
				else error = "Invalid extension, try again!";
			}
			else goto endOfProgram;
		}*/

		Controller controller{ repo, list };
		//controller.load();
		QtGUI_Hybris w(controller);
		w.show();
		ret = a.exec();
		delete repo;
		delete list;
	}
	//{
	//	/*HANDLE thread = CreateThread(NULL, 0, startProcess, file, 0, NULL);
	//	WaitForSingleObject(thread, INFINITE);
	//	CloseHandle(thread);*/
	//	//}
	//	
	//	std::cout << "Enter file path (if custom path is needed) or name (if you want to create it in the current directory): " << '\n';
	//	std::string path = "file.txt";
	//	std::getline(std::cin, path);
	//	RepoInterface* repo;
	//	while (true)
	//	{
	//		std::getline(std::cin, path);
	//		if (path == "inmemory")
	//		{
	//			repo = new RepoInMemory();
	//			break;
	//		}
	//		else if (path.find(".db") != std::string::npos)
	//		{
	//			repo = new DBRepository(path);
	//			break;
	//		}
	//		else if (path.find(".txt") != std::string::npos || path.find(".csv") != std::string::npos)
	//		{
	//			repo = new FileRepository(path);
	//			break;
	//		}
	//		else if (path.find(".html") != std::string::npos)
	//		{
	//			repo = new HTMLRepo(path);
	//			break;
	//		}
	//		else
	//		{
	//			std::cout << "Invalid extension, try again!" << std::endl;
	//		}
	//	}
	//	std::cout << "Enter file path for your list (.csv if csv file, .html if html file): " << '\n';
	//	std::string myList;
	//	std::getline(std::cin, myList);
	//	if (myList.find(".csv") != std::string::npos || myList.find(".txt") != std::string::npos)
	//	{
	//		FileRepository list(myList);
	//		Controller controller{ *repo, list };
	//		UI ui{ controller };
	//		ui.start();
	//	}
	//	else if (myList.find(".html") != std::string::npos)
	//	{
	//		HTMLRepo list(myList);
	//		Controller controller{ *repo, list };
	//		UI ui{ controller };
	//		ui.start();
	//	}
	//	delete repo;
	//}
endOfProgram:
	if (listpath != "")
	{
		remove(listpath.c_str());
	}
#if defined(WIN32) && defined(_DEBUG)
	setFilterDebugHook();
#endif
	_CrtDumpMemoryLeaks();
	return ret != 0 ? ret : 0;
}