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

#pragma once

#include <cstdint>

namespace LCDDisplay {
	class View {
	protected:
		int borderLeftWidth=2;
		int borderRightWidth=2;
		int borderTopWidth=2;
		int borderBottomWidth=2;
		View* parent = 0;
		View** children = 0;
		int childCount = 0;
		virtual void drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color);
		virtual void drawPixel(int xLoc, int yLoc, uint32_t color);
		void drawBorders();
		void drawBg();
		// void drawBorder(borders border);
		int xViewLoc;
		int yViewLoc;
		int viewHeight;
		int viewWidth;
		enum hAlign {alignLeft, alignRight, alignCentre};
		enum vAlign {alignTop, alignBottom, alignMiddle};
		uint32_t bgColor = 0x303030;
		uint32_t borderColor = 0xffffff;
		void childWasRemoved(View* child);
		void drawChild(View* child);
		void drawAllChildren();
	public:
		enum borders {borderLeft=1, borderRight=2, borderTop=4, borderBottom=8};
		View(int xLoc, int yLoc);
		View(int xLoc, int yLoc, int xSize, int ySize);
		~View();
		void resize(int xSize, int ySize);
		void move(int xLoc, int yLoc);
		void setBorders(borders border, int size); // Pass -1 to keep current
		virtual void draw();
		virtual void updateArea(int xLoc, int yLoc, int xSize, int ySize);
		void setBgColor(uint32_t color);
		void setBorderColor(uint32_t color);
		void addChild(View* view);
		View* getParent();
		View** getChildrenArrayAddress();
		int getChildCount();
	};
} /* LCDDisplay */
