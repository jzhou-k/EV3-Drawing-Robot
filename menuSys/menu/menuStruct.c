#pragma once
const int MAX_MENU_LENGTH = 16;
const int MAX_DISPLAY_HEIGHT = 8;
const int WAIT_TIME_AFTER_MENU_UPDATE = 500;
struct menuList
{
	int topPos;
	int index;
	unsigned long lastUpdate;
	string indexNames[MAX_MENU_LENGTH];
};

void menuConstruct(struct menuList &self)
{
	self.topPos = 0;
	self.index = 0;
	for (int name = 0; name < MAX_MENU_LENGTH; name++)
	{
		self.indexNames[name] = "\0";
	}
}

int getLengthMenu(const struct menuList &self)
{
	int addr = 0;
	string nullStr = "\0";
	for (; addr < MAX_MENU_LENGTH; addr++)
	{
		if (strcmp(self.indexNames[addr], nullStr) == 0)
		{
			return addr;
		}
	}
	return addr;
}

void displayMenu(const struct menuList &self)
{
	eraseDisplay();
	for (int line = 0; (line < MAX_DISPLAY_HEIGHT && line < getLengthMenu(self)); line++)
	{
		displayBigTextLine(line * 2, "%s%s", (line + self.topPos) == self.index ? "> " : "  ", self.indexNames[line + self.topPos]);
	}
}

bool displayPointerUpdate(struct menuList &self)
{
	if (nPgmTime - self.lastUpdate > WAIT_TIME_AFTER_MENU_UPDATE)
	{
		if (getButtonPress(buttonUp) == 1 && self.index > 0)
		{
			self.index--;
			if (self.index < self.topPos)
			{
				self.topPos--;
			}
			self.lastUpdate = nPgmTime;
			return true;
		}
		else if (getButtonPress(buttonDown) == 1 && self.index < (getLengthMenu(self) - 1))
		{
			self.index++;
			if (self.index >= MAX_DISPLAY_HEIGHT + self.topPos)
			{
				self.topPos++;
			}
			self.lastUpdate = nPgmTime;
			return true;
		}
	}
	return false;
}
