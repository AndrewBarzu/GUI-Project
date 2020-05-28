#include "UI.h"
#include <iostream>
#include <cstring>
#include <Windows.h>
#include "setDebugNew.h"
#define new DEBUG_NEW

void command_parser(char* command, char returnValue[7][100])
{
	char* token = strtok(command, ", \n");
	int index = 0;

	while (token != NULL)
	{
		strncpy(returnValue[index], token, 99);
		int i = 0;
		while (returnValue[index][i] == ' ')
			i++;
		strncpy(returnValue[index], returnValue[index] + i, 99);
		for (int i = strlen(returnValue[index]) - 1; returnValue[index][i] == ' '; i--)
			returnValue[index][i] = '\0';
		returnValue[index][99] = 0;
		token = strtok(NULL, ",\n");
		index++;
	}
}

void UI::start()
{
	/*controller.change_mode("A");
	char aura_gen[4] = "100";
	char parts_gen[4] = "300";
	char loc_gen[4] = "yey";
	char size_gen[4] = "big";
	char vision_gen[4] = "kek";
	for (int i = 1; i <= 9; i++)
	{
		std::cout << loc_gen << " "<< size_gen << " " << aura_gen << " " << parts_gen << " " << vision_gen <<"\n";
		if (i % 2 == 0)
			aura_gen[2] += 1;
		if (i % 3 == 0)
			parts_gen[2] += 2;
		loc_gen[1] += 1;
	}*/
	while (1)
	{
		if (controller.getMode() == "A")
		{
			printf("\nThe commands are as follows:\n\n\
mode <string>: changes the mode to the given new mode\n\n\
add <string> <string> <int> <int> <string>: adds a tower with the given location, size, aura level, number of parts and vision\n\n\
update <string> <string> <int> <int> <string>: updates the tower with the given location, to have the given size, aura level, number of parts and vision\n\n\
delete <string>: deletes the at the given location\n\n\
list: lists all elements\n\n\
load: adds 10 entries to the current container. USE ONLY IF EMPTY\n\n\
exit: intuitive i guess\n");
		}
		else if (controller.getMode() == "B")
		{
			printf("\nThe commands are as follows:\n\n\
mode <string>: changes the mode to the given new mode\n\n\
list <string>: iterate through the towers that have a given size. Leave empty if no filtering is wanted\n\n\
save <string>: save the turret at the given location\n\n\
next: displays the next turret\n\n\
mylist: displays all saved turrets\n\n\
exit: intuitive i guess\n");
		}
		std::cout << '\n';
		int validCommand = 0;
		char command[500];
		fgets(command, 500, stdin);
		system("CLS");
		for (int i = 0; i < 500 && validCommand == 0; i++)
		{
			if (command[i] == '\n')
				validCommand = 1;
		}

		if (validCommand == 0)
		{
			printf("%s\n", "Command is too long!");
			continue;
		}
		char parsedCommand[7][100];
		for (int i = 0; i < 7; i++)
		{
			memset(parsedCommand[i], '\0', 100 * sizeof(char));
		}
		command_parser(command, parsedCommand);

		int retCode = 0;
		if (strcmp(parsedCommand[0], "exit") == 0)
		{
			break;
		}
		else if (strcmp(parsedCommand[0], "mode") == 0)
		{
			std::string myMode{ parsedCommand[1][0] };
			int change = controller.change_mode(myMode);
			if (change == 0)
			{
				system("CLS");
			}
		}
		else if (strcmp(parsedCommand[0], "add") == 0)
		{
			/*bool flag = true;
			for (int i = 0; parsedCommand[3][i] != 0 && flag == true; i++)
			{
				if (!isdigit(parsedCommand[3][i]))
					flag = false;
			}
			for (int i = 0; parsedCommand[4][i] != 0 && flag == true; i++)
			{
				if (!isdigit(parsedCommand[4][i]))
					flag = false;
			}*/
			try
			{
				controller.add(parsedCommand[1], parsedCommand[2], parsedCommand[3], parsedCommand[4], parsedCommand[5]);
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (strcmp(parsedCommand[0], "update") == 0)
		{
			/*bool flag = true;
			for (int i = 0; parsedCommand[3][i] != 0 && flag == true; i++)
			{
				if (!isdigit(parsedCommand[3][i]))
					flag = false;
			}
			for (int i = 0; parsedCommand[4][i] != 0 && flag == true; i++)
			{
				if (!isdigit(parsedCommand[4][i]))
					flag = false;
			}*/
			try
			{
				controller.update(parsedCommand[1], parsedCommand[2], parsedCommand[3], parsedCommand[4], parsedCommand[5]);
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (strcmp(parsedCommand[0], "delete") == 0)
		{
			try
			{
				controller.remove(parsedCommand[1]);
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (strcmp(parsedCommand[0], "list") == 0)
		{
			if (parsedCommand[1][0] == '\0' && controller.getMode() == "A")
				for (auto& iter: this->controller.print())
				{
					std::cout << iter << '\n';
				}
			else
			{
				std::string res;
				try
				{
					res = controller.list(parsedCommand[1]);
				}
				catch (const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
				std::cout << res << '\n';
			}
		}
		else if (strcmp(parsedCommand[0], "next") == 0)
		{
			system("CLS");
			std::string result;
			try
			{
				result = controller.next();
			}
			catch (const ModeException& e)
			{
				std::cerr << e.what() << '\n';
			}
			std::cout << result << '\n';
		}
		else if (strcmp(parsedCommand[0], "save") == 0)
		{
			controller.save(parsedCommand[1]);
		}
		else if (strcmp(parsedCommand[0], "mylist") == 0)
		{
			this->controller.mylist();
		}
		else if (strcmp(parsedCommand[0], "load") == 0)
		{
			try {
				this->controller.load();
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		
		if (retCode != 0)
			std::cout<< "error!\n";
	}
}