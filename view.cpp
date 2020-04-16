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
	if (xSize<xOldSize) {
		// Update for horizontal shrinkage
		parent->updateArea(xViewLoc+xSize, yViewLoc ,xOldSize-xSize, yOldSize);
	}
	if (ySize<yOldSize) {
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
void View::setBorders(borders border, int size) {
	// left right top bottom
	if ((border&1)!=0) {
		borderLeftWidth=size;
	}
	if ((border&2)!=0) {
		borderRightWidth=size;
	}
	if ((border&4)!=0) {
		borderTopWidth=size;
	}
	if ((border&8)!=0) {
		borderBottomWidth=size;
	}
	drawBorders();
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
	drawRect(borderLeftWidth,borderTopWidth,viewWidth-borderLeftWidth-borderRightWidth,
		viewHeight-borderTopWidth-borderBottomWidth, bgColor);
	drawAllChildren();
}
void View::setBorderColor(uint32_t color) {
	borderColor=color;
	drawBorders();
}
void View::drawBorders() {
	drawRect(0,0,borderLeftWidth,viewHeight, borderColor);
	drawRect(0,0,viewWidth,borderTopWidth, borderColor);
	drawRect(0,viewHeight-borderBottomWidth,viewWidth,borderBottomWidth, borderColor);
	drawRect(viewWidth-borderRightWidth,0,borderRightWidth,viewHeight, borderColor);
}
void View::updateArea(int xLoc, int yLoc, int xSize, int ySize) {
	if (xLoc <= borderLeftWidth) {
		drawRect(xLoc, yLoc, borderLeftWidth-xLoc, ySize, borderColor);
	}
	if (yLoc <= borderTopWidth) {
		drawRect(xLoc, yLoc, xSize, borderTopWidth-yLoc, borderColor);
	}
	if (xLoc+xSize > viewWidth-borderRightWidth) {
		drawRect(viewWidth-borderRightWidth, yLoc, viewWidth-(xLoc+xSize), ySize, borderColor);
	}
	if (yLoc+ySize > viewHeight-borderBottomWidth) {
		drawRect(xLoc, viewHeight-borderBottomWidth, xSize, viewHeight-(ySize+yLoc), borderColor);
	}
	drawRect(xLoc, yLoc, xSize, ySize, bgColor);
	for (int i=0; i<childCount; i++) {
		if (xLoc<=children[i]->xViewLoc + children[i]->viewWidth &&
			xLoc+xSize>=children[i]->xViewLoc &&
			yLoc<=children[i]->yViewLoc + children[i]->viewHeight &&
			yLoc+ySize>=children[i]->yViewLoc)
		{
			children[i]->draw();
		}
	}
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
void View::addChild(View* child) {
	if (childCount>=64) {
		printf("View::addChild: view already has max children\n");
		return;
	}
	child->parent=this;
	children[childCount]=child;
	this->childCount++;
	child->draw();
}
void View::childWasRemoved(View* child) {
	bool childWasFound=false;
	for (int i = 0; i<childCount; i++) {
		if (children[i]==child) {
			childWasFound=true;
		}
		if (childWasFound==true) {
			children[i]=children[i+1];
		}
	}
	this->childCount--;
	if (childWasFound==false) {
		printf("View::childWasRemoved: could not find matching child\n");
	}
}
