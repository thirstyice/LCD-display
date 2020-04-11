#include "screen.h"

using namespace LCDDisplay;

Screen::Screen(FrameBuffer* initBuffer) {
	buffer=initBuffer;

	drawRect(0,0,buffer->getXres(), buffer->getYres(), bgColor);
}
Screen::~Screen() {

}
void Screen::drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color) {
	if (xSize==0||ySize==0) {
		return;
	}
	if (xSize==1&&ySize==1) {
		Screen::drawPixel(xLoc,yLoc,color);
		return;
	}
	uint8_t blue = color;
	uint8_t green = color >> 8;
	uint8_t red = color >> 16;
	if (xLoc+xSize>buffer->getXres()) {
		xSize=buffer->getXres()-xLoc;
	}
	if (yLoc+ySize>buffer->getYres()) {
		ySize=buffer->getYres()-yLoc;
	}
	if (xLoc==0&&xSize==buffer->getXres()) {
		// We can draw in one command
		int start = yLoc*buffer->getXres()+xLoc;
		int end = (yLoc+ySize)*buffer->getXres();
		buffer->draw(start, end, red, green, blue);
	} else {
		for (int y=0;y<ySize;y++) {
			buffer->draw((yLoc+y)*buffer->getXres()+xLoc,(yLoc+y)*buffer->getXres()+xLoc+xSize, red, green, blue);
		}
	}
}
void Screen::drawPixel(int xLoc, int yLoc, uint32_t color) {
	if (xLoc>buffer->getXres()||xLoc<0||yLoc>buffer->getYres()||yLoc<0) {
		return;
	}
	uint8_t blue = color;
	uint8_t green = color >> 8;
	uint8_t red = color >> 16;
	int pixel = yLoc*buffer->getXres()+xLoc;
	buffer->draw(pixel, pixel+1, red, green, blue);
}
