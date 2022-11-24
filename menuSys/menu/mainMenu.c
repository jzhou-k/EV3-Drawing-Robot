#pragma once
#include "menuStruct.c"
#include "moveAxiesMenu.c"
#include "filePickerMenu.c"
void mainMenuSetup(struct menuList &self)
{
	const int MAIN_MENU_LENGTH = 3;
	menuConstruct(self);
	string indexNames[MAIN_MENU_LENGTH] = {"Run file", "Move axies", "Exit"}; //,"d","e","f","g","h","i","j","k","l","m","n","o","p"
	for (int name = 0; name < MAIN_MENU_LENGTH; name++)
	{
		self.indexNames[name] = indexNames[name];
	}
	self.lastUpdate = nPgmTime;
}

/*
	ROBOTC MANIFESTO
	There are literally no function pointers in this language
	Why
	Also can't use swtich statments because when I tried that
	the robotC ide crashed from the text idk anymore
	This is the reasion the function below exists
*/

int mainMenuExecute(struct menuList &self)
{
	if (getButtonPress(ENTER_BUTTON))
	{
		while (getButtonPress(ENTER_BUTTON))
		{
		}
		switch (self.index)
		{
		case 0:
			filePickerMenuRun();
			return 2;
		case 1:
			moveAxiesMenuRun();
			return 2;
		case 2:
			return 1;
		}
	}
	return -1;
}

void mainMenuRun()
{
	bool exitProgram = false;
	bool displayUpdate = false;
	menuList mainMenu;
	mainMenuSetup(mainMenu);
	displayMenu(mainMenu);
	while (!exitProgram)
	{
		displayUpdate = displayPointerUpdate(mainMenu);
		switch (mainMenuExecute(mainMenu))
		{
		case 1:
			exitProgram = true;
		case 2:
			displayUpdate = true;
		}
		if (displayUpdate)
		{
			displayMenu(mainMenu);
			displayUpdate = false;
		}
	}
}
