#pragma once

#include <cstdint>

namespace LCDDisplay {
	class View {
	protected:
		int borderLeft=2;
		int borderRight=2;
		int borderTop=2;
		int borderBottom=2;
		//View* parent;
		virtual void drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color);
		virtual void drawPixel(int xLoc, int yLoc, uint32_t color);
		void drawBorders();
		// void drawBorder(borders border);
		int xViewLoc;
		int yViewLoc;
		int viewHeight;
		int viewWidth;
		enum hAlign {alignLeft, alignRight, alignCentre};
		enum vAlign {alignTop, alignBottom, alignMiddle};
		uint32_t bgColor = 0x303030;
		uint32_t borderColor = 0xffffff;
	public:
		// enum borders {borderLeft, borderRight, borderTop, borderBottom};
		View(int xLoc, int yLoc);
		View(int xLoc, int yLoc, int xSize, int ySize);
		~View();
		void resize(int xSize, int ySize);
		void move(int xLoc, int yLoc);
		void setBorder(int top, int bot, int left, int right); // Pass -1 to keep current
		virtual void draw();
		void setBgColor(uint32_t color);
		void setBorderColor(uint32_t color);
		View* addChild();
		View* getParent();
	};
} /* LCDDisplay */
