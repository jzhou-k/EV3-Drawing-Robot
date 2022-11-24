#include "menuStruct.c"
#include "mainMenu.c"
bool exitProgram = false;

// menu struct start

// menu struct end

// main menu struct declaration

task main()
{
	//int *fun_ptr() = &a;
	bool displayUpdate = false;
	menuList mainMenu;
	mainMenuSetup(mainMenu);
	//mainMenu.topPos = mainMenu.index = 2;
	while(!exitProgram){
		displayUpdate = displayPointerUpdate(mainMenu);
		mainMenuExecute(mainMenu);
		if (displayUpdate){
			displayMenu(mainMenu);
			displayUpdate = false;
		}
	}
}
