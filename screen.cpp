#include "screen.h"

using namespace LCDDisplay;

Screen::Screen(FrameBuffer* initBuffer) {
	buffer=initBuffer;
	screenArray = new uint32_t*[buffer->getXres()];
	for (int i = 0; i<buffer->getXres(); i++) {
		screenArray[i] = new uint32_t[buffer->getYres()];
	}
	for (int x=0; x<buffer->getXres(); x++) {
		for (int y=0; y<buffer->getYres(); y++) {
				screenArray[x][y] = bgColor;
		}
	}
	drawRect(0,0,buffer->getXres(), buffer->getYres(), bgColor);
}
Screen::~Screen() {
	for (int i=0;i<buffer->getXres(); i++) {
		delete[] screenArray[i];
	}
	delete[] screenArray;
}
void Screen::drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color) {
	printf("Drawing at:%i,%i size:%i,%i res:%i,%i\n",xLoc,yLoc,xSize,ySize,buffer->getXres(),buffer->getYres() );
	uint8_t blue = color;
	uint8_t green = color >> 8;
	uint8_t red = color >> 16;
	if (xLoc+xSize>buffer->getXres()) {
		xSize=buffer->getXres()-xLoc;
	}
	if (yLoc+ySize>buffer->getYres()) {
		ySize=buffer->getYres()-yLoc;
	}
	//Update the screenArray
	for (int y=yLoc; y<ySize; y++) {
		for (int x=xLoc; x<xSize; x++) {
			screenArray[x][y]=color;
		}
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
	uint8_t blue = color;
	uint8_t green = color >> 8;
	uint8_t red = color >> 16;
	// Update the screen array
	screenArray[xLoc][yLoc]=color;
	int pixel = yLoc*buffer->getXres()+xLoc;
	buffer->draw(pixel, pixel+1, red, green, blue);
}
