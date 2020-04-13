#include "application.h"

#include <iostream>

using namespace LCDDisplay;

int main(int argc, char const *argv[]) {
	Application app = Application();
	View testView = View(10,10);
	testView.setBgColor(0xff0000);
	app.mainView->addChild(&testView);
	std::cin.ignore();
	testView.resize(100,100);
	return 0;
}
