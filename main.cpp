#include "display/screen.h"
#include "display/view.h"

#include <iostream>

using namespace LCDDisplay;

int main(int argc, char const *argv[]) {
	View* screen = new Screen();
	View testView = View(-5,-5,50,50);
	testView.setBgColor(0xff0000);
	screen->addChild(&testView);
	std::cin.ignore();
	View view2 = View(10,10,50,50);
	view2.setBgColor(0x00ff00);
	testView.addChild(&view2);
	std::cin.ignore();
	testView.move(200,100);
	std::cin.ignore();
	testView.setBorders(View::borderLeft+View::borderRight, 10);
	return 0;
}
