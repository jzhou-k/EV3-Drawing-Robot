#pragma once
#include "menuStruct.c"
#include "../lib/EV3_FileIO.c"
#include "../draw/test.c"

// sets up menu struct for the file picker menu
void filePickerMenuSetup(struct menuList &self)
{
	const int FILE_PICKER_MENU_LENGTH = 1;
	menuConstruct(self);
	string indexNames[FILE_PICKER_MENU_LENGTH] = {"Back"};
	for (int name = 0; name < FILE_PICKER_MENU_LENGTH; name++)
	{
		self.indexNames[name] = indexNames[name];
	}
	self.lastUpdate = nPgmTime;
}

// tells the menu what function to execute depending on index position in menu
int filePickerMenuExecute(struct menuList &self)
{
	if (getButtonPress(ENTER_BUTTON))
	{
		while (getButtonPress(ENTER_BUTTON))
		{
		}
		switch (self.index)
		{
		case 0:
			return 1;
			break;
		default:
			return 3;
			break;
		}
	}
	return -1;
}

bool checkFile(string fileName)
{
	TFileHandle fin;
	openReadPC(fin, fileName);
	string tempVal;
	bool foundFile = readTextPC(fin, tempVal);
	closeFilePC(fin);
	return foundFile;
}

// changes menu structs pointer update behavior such that it works with infinite amount of files on EV3 if files are labeled from 0.txt to n.txt
void fileMenuIntegrator(struct menuList &self, int &currentMinFile, bool init = false)
{
	string currentMaxFile;
	const int MAX_FILE_DISPLAY = 10;

	string fileName;
	bool updateFiles = init;
	stringFormat(currentMaxFile, "%d.txt", currentMinFile + MAX_FILE_DISPLAY);

	if (self.index == MAX_DISPLAY_HEIGHT + 2 && self.topPos == 3 && checkFile(currentMaxFile))
	{
		self.index--;
		self.topPos--;
		currentMinFile++;
		updateFiles = true;
	}
	else if (self.index == 1 && self.topPos == 1 && currentMinFile != 0)
	{
		self.index++;
		self.topPos++;
		currentMinFile--;
		updateFiles = true;
	} /**/

	int numFiles = currentMinFile;
	stringFormat(fileName, "%d.txt", numFiles);

	if (updateFiles)
	{
		for (int line = 1; line < MAX_MENU_LENGTH; line++)
		{
			self.indexNames[line] = "\0";
		}
		for (; (numFiles < MAX_FILE_DISPLAY + currentMinFile && checkFile(fileName)); ++numFiles)
		{
			stringFormat(fileName, "%d.txt", numFiles);
			string tempFileName;
			stringFormat(tempFileName, "%d.txt", numFiles);
			self.indexNames[numFiles - currentMinFile + 1] = tempFileName;
			updateFiles = false;
		}
	}
}

void filePickerMenuRun()
{
	bool exitProgram = false;
	bool displayUpdate = false;
	int currentMinFile = 0;
	menuList filePickerMenu;
	filePickerMenuSetup(filePickerMenu);
	fileMenuIntegrator(filePickerMenu, currentMinFile, true);
	displayMenu(filePickerMenu);
	while (!exitProgram)
	{
		displayUpdate = displayPointerUpdate(filePickerMenu);
		switch (filePickerMenuExecute(filePickerMenu))
		{
			case 1:
				exitProgram = true;
				break;
			case 2:
				displayUpdate = true;
				break;
			case 3:
				string fileName = "";
				stringFormat(fileName, "%d.txt", currentMinFile+filePickerMenu.index-1);
				EV3draw(fileName);
				displayUpdate = true;
				break;
		}
		if (displayUpdate)
		{
			fileMenuIntegrator(filePickerMenu, currentMinFile);
			displayMenu(filePickerMenu);
			displayUpdate = false;
		}
	}
}
