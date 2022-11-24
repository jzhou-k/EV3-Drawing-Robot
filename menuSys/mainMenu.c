#pragma once
#include "menuStruct.c"
void mainMenuSetup(struct menuList &self) {
	self.topPos = 0;
	self.index = 0;
	string indexNames[MAX_MENU_LENGTH] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p"};
	for(int name = 0; name<MAX_MENU_LENGTH; name++) {
		self.indexNames[name] = indexNames[name];
	}
	self.lastUpdate = nPgmTime;
}

/*
	There are literally no function pointers in this language
	Why
	This is the reasion the function below exists
*/

// collisions are probably high
int basicHash(string input){
	int hash = 5000;
	int* strP = &input;
	for(int strLength = 0; strLength<20; strLength++){
		hash = ((hash << 10)+hash)+strP[strLength];
	}
	return hash;
}

int fuck;

bool mainMenuExecute(struct menuList &self){
	if(getButtonPress(ENTER_BUTTON)){
		while(getButtonPress(ENTER_BUTTON)){}
		switch(basicHash(self.indexNames[self.index])){
			case 1940978962:
	      playTone(484, 50);
	      break; /* optional */

   		case -961432298:
	      playTone(584, 50);
	      break; /* optional */

   		/* you can have any number of case statements */
   		default : /* Optional */
   			displayBigTextLine(0,"%d", basicHash(self.indexNames[self.index]));
   			fuck = basicHash(self.indexNames[self.index]);
				wait1Msec(5000);
   			playTone(784, 50);
		}
  }
  return false;
}
