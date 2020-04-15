//
// Copyright (C) 2020 Tauran Wood
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>
//

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
 if (parent) {
	 parent->childWasRemoved(this);
 }
}
void View::resize(int xSize, int ySize) {
	int xOldSize=viewWidth;
	int yOldSize=viewHeight;
	viewWidth=xSize;
	viewHeight=ySize;
	draw();
	if (xSize<viewWidth) {
		// Update for horizontal shrinkage
		parent->updateArea(xViewLoc+xSize, yViewLoc ,xOldSize-xSize, yOldSize);
	}
	if (ySize<viewHeight) {
		// Update for vertical shrinkage
		parent->updateArea(xViewLoc, yViewLoc+ySize, xOldSize, yOldSize-ySize);
	}

}
void View::move(int xLoc, int yLoc) {
	int xOldLoc=xViewLoc;
	int yOldLoc=yViewLoc;
	xViewLoc=xLoc;
	yViewLoc=yLoc;
	draw();
	parent->updateArea(xOldLoc, yOldLoc, viewWidth, viewHeight);
}
void View::setBorders(int top, int bot, int left, int right) {
	// TODO
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
	drawAllChildren();
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
	if (parent) {
		parent->drawPixel(xLoc, yLoc, color);
	}
}
View** View::getChildrenArrayAddress() {
	return children;
}
int View::getChildCount() {
	return childCount;
}
void View::drawChild(View* child) {
	child->draw();
}
void View::drawAllChildren() {
	for (int child=0; child<childCount; child++) {
		drawChild(children[child]);
	}
}
void View::addChild(View* view) {
	view->parent=this;
	// TODO add child to pointer array
	this->childCount++;
	view->draw();
}
void View::childWasRemoved(View* child) {
	// TODO remove child from pointer array
	this->childCount--;
}
