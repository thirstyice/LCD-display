#include "application.h"

#include <iostream>

using namespace LCDDisplay;

int main(int argc, char const *argv[]) {
	Application app = Application();
	View testView = View(10,10,50,50);
	testView.setBgColor(0xff0000);
	app.mainView->addChild(&testView);
	std::cin.ignore();
	View view2 = View(10,10,50,50);
	view2.setBgColor(0x00ff00);
	testView.addChild(&view2);
	std::cin.ignore();
	testView.move(200,100);
	return 0;
}
