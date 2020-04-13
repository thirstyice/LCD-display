#include "view.h"

#include <cstdio>

using namespace LCDDisplay;

View::View(int xSize, int ySize) {
	xViewLoc=50;
	yViewLoc=50;
	viewWidth=xSize;
	viewHeight=ySize;
}
View::View(int xLoc, int yLoc, int xSize, int ySize) {
	xViewLoc=xLoc;
	yViewLoc=yLoc;
	viewWidth=xSize;
	viewHeight=ySize;
}
View::~View() {

}
void View::resize(int xSize, int ySize) {
	bool needsParentUpdate = true;
	if  (xSize>=viewWidth&&ySize>=viewHeight) {
		// We don't need to worry about telling parent to redraw areas that were covered
		needsParentUpdate=false;
	}
	viewWidth=xSize;
	viewHeight=ySize;
	draw();
	if (needsParentUpdate) {
		//TODO
	}

}
void View::move(int xLoc, int yLoc) {

}
void View::setBorder(int top, int bot, int left, int right) {

} // Pass -1 to keep current
void View::draw() {
	drawBg();
	drawBorders();
}
void View::setBgColor(uint32_t color) {
	bgColor=color;
	drawBg();
}
void View::drawBg() {
	drawRect(borderLeft,borderTop,viewWidth-borderLeft-borderRight,
		viewHeight-borderTop-borderBottom, bgColor);
	// TODO: redraw children
}
void View::setBorderColor(uint32_t color) {
	borderColor=color;
	drawBorders();
}
void View::drawBorders() {
	drawRect(0,0,borderLeft,viewHeight, borderColor);
	drawRect(0,0,viewWidth,borderTop, borderColor);
	drawRect(0,viewHeight-borderBottom,viewWidth,borderBottom, borderColor);
	drawRect(viewWidth-borderRight,0,borderRight,viewHeight, borderColor);
}
void View::updateArea(int xLoc, int yLoc, int xSize, int ySize) {
	if (xLoc <= borderLeft) {
		drawRect(xLoc, yLoc, borderLeft-xLoc, ySize, borderColor);
	}
	if (yLoc <= borderTop) {
		drawRect(xLoc, yLoc, xSize, borderTop-yLoc, borderColor);
	}
	if (xLoc+xSize > viewWidth-borderRight) {
		drawRect(viewWidth-borderRight, yLoc, viewWidth-(xLoc+xSize), ySize, borderColor);
	}
	if (yLoc+ySize > viewHeight-borderBottom) {
		drawRect(xLoc, viewHeight-borderBottom, xSize, viewHeight-(ySize+yLoc), borderColor);
	}
	drawRect(xLoc, yLoc, xSize, ySize, bgColor);

	// TODO Redraw children that overlap with area
}
void View::drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color) {
	if (parent) {
		parent->drawRect(xViewLoc+xLoc,yViewLoc+yLoc, xSize, ySize, color);
	}
}
void View::drawPixel(int xLoc, int yLoc, uint32_t color) {

}
View** View::getChildren() {
	//TODO
	return 0;
}
void View::addChild(View* view) {
	view->parent=this;
	view->draw();
}
