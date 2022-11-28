#pragma once
#include "menuStruct.c"

// sets up menu struct for the move axies menu
void moveAxiesMenuSetup(struct menuList &self)
{
	const int MOVE_AXIES_MENU_LENGTH = 4;
	menuConstruct(self);
	string indexNames[MOVE_AXIES_MENU_LENGTH] = {"Back", "Move X axis", "Move Y axis", "Move Z axis"};
	for (int name = 0; name < MOVE_AXIES_MENU_LENGTH; name++)
	{
		self.indexNames[name] = indexNames[name];
	}
	self.lastUpdate = nPgmTime;
}

// tells the menu what function to execute depending on index position in menu
bool moveAxiesMenuExecute(struct menuList &self)
{
	if (getButtonPress(ENTER_BUTTON))
	{
		while (getButtonPress(ENTER_BUTTON))
		{
		}
		switch (self.index)
		{
		case 0:
			return false;
			break;
		case 1:
			playTone(584, 50);
			return true;
			break;
		case 2:
			playTone(484, 50);
			return true;
			break;
		}
		return false;
	}
	return true;
}

void moveAxiesMenuRun()
{
	bool exitProgram = false;
	bool displayUpdate = false;
	menuList moveAxiesMenu;
	moveAxiesMenuSetup(moveAxiesMenu);
	displayMenu(moveAxiesMenu);
	while (!exitProgram)
	{
		displayUpdate = displayPointerUpdate(moveAxiesMenu);
		exitProgram = !moveAxiesMenuExecute(moveAxiesMenu);
		if (displayUpdate)
		{
			displayMenu(moveAxiesMenu);
			displayUpdate = false;
		}
	}
}
