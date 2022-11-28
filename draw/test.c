#pragma once

void EV3draw(string fileName) {
	eraseDisplay();
	displayBigTextLine(0, fileName);
	while(!getButtonPress(ENTER_BUTTON)){}
	while(getButtonPress(ENTER_BUTTON)){}
}
