#pragma once
#include "menuStruct.c"
#include "../lib/EV3_FileIO.c"

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
		case 1:
			playTone(584, 50);
			return 2;
		case 2:
			playTone(484, 50);
			return 2;
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
// string fileName;
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
			// displayString(0,"%s %d",tempFileName, numFiles-currentMinFile+1);
			// wait1Msec(2000);
			self.indexNames[numFiles - currentMinFile + 1] = tempFileName;
			updateFiles = false;
		}
	}
}

// int currentMinFile = 0;
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
		case 2:
			displayUpdate = true;
		}
		if (displayUpdate)
		{
			fileMenuIntegrator(filePickerMenu, currentMinFile);
			displayMenu(filePickerMenu);
			displayUpdate = false;
		}
	}
}
