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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "screen.h"

using namespace LCDDisplay;

Screen::Screen(const char* buffer):View(0,0,fbVarInfo.xres,fbVarInfo.yres) {
	int fbDevice=0;
	// Open the file for reading and writing
	fbDevice = open(buffer, O_RDWR);
	if (!fbDevice) {
		throw("Error: cannot open framebuffer device %s\n", buffer);
	}
	// Get variable screen information
	if (ioctl(fbDevice, FBIOGET_VSCREENINFO, &fbVarInfo)) {
		throw("Error reading variable information.\n");
	}
	// Get fixed screen information
	if (ioctl(fbDevice, FBIOGET_FSCREENINFO, &fbFixInfo)) {
		throw("Error reading fixed information.\n");
	}
	// map framebuffer to user memory
	fbSize = fbFixInfo.smem_len;

	fbPointer = (unsigned char*)mmap(0,fbSize,PROT_READ | PROT_WRITE,MAP_SHARED,fbDevice, 0);
	if ((int)fbPointer == -1) {
		throw("Failed to mmap.\n");
	}
	// cleanup
	close(fbDevice);

	viewWidth=fbVarInfo.xres;
	viewHeight=fbVarInfo.yres;

	draw();
}
Screen::~Screen() {
	munmap(fbPointer, fbSize);
}
void Screen::bufferDraw(int startPixel, int endPixel, uint8_t red, uint8_t green, uint8_t blue) {
	if (startPixel > endPixel) {
		// Allow bufferDrawing reversed lines
		int tmp = startPixel;
		startPixel = endPixel;
		endPixel = tmp;
	}
	uint8_t bytesPerPixel=fbVarInfo.bits_per_pixel/8;
	if (fbVarInfo.bits_per_pixel%8 != 0) {
		// int division defaults to floor, we want it to ceil
		bytesPerPixel++;
	}
	unsigned char * start = fbPointer+(startPixel*bytesPerPixel);
	size_t length = endPixel-startPixel;
	switch (fbVarInfo.bits_per_pixel) { // More color pallettes to be implemented
		case 8: {
			uint8_t color = ((red/32)<<5) + ((green/32) <<2) + blue/64;
			memset(start, color, length);
			break;

		} case 16: {
		// 	uint8_t colors[2] = {((blue/4)<<2) + ((red/8)>>3), (green/8) + ((red/8)<<5)};
		// 	for (int pixel=0;pixel<length;pixel++){
		// 		for (uint8_t byte=0; byte<2;byte++) {
		// 			memset(start+(pixel*bytesPerPixel)+byte, colors[byte], 1);
		// 		}
		// 	}
		uint8_t color= (green/32) + ((blue/64) << 3) + ((red / 32) << 5);
#ifdef DEBUG
		printf("Drawing at:%i for:%i ending:%i\n",start,length*2,start+(length*2));
#endif //DEBUG
		memset(start, color, length*2);
		break;
		} case 32: {
			uint8_t colors[4] = {red, red, green, blue};
			for (int pixel=0;pixel<length;pixel++){
				for (uint8_t byte=0; byte<4;byte++) {
					uint8_t colorByte = colors[byte];
					memset(start+(pixel*bytesPerPixel)+byte, colorByte, 1);
				}
			}
			break;
		}
		default: {
			printf("Unknown colorspace: %i", fbVarInfo.bits_per_pixel);
			return;
		}
	}
}
void Screen::drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color) {
	if (xLoc>=viewWidth||yLoc>=viewHeight||(xLoc+xSize)<0||(yLoc+ySize)<0) {
		// Don't even bother trying to draw rectangles that are outside the visible area
		return;
	}
	if (xLoc<0) {
		xSize+=xLoc;
		xLoc=0;
	}
	if (yLoc<0) {
		ySize+=yLoc;
		yLoc=0;
	}
	if ((xLoc+xSize)>=(viewWidth)) {
		xSize=viewWidth-xLoc;
	}
	if ((yLoc+ySize)>=viewHeight) {
		ySize=viewHeight-yLoc;
	}
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
	if (xLoc==0&&xSize==viewWidth) {
		// We can draw in one command
		int start = yLoc*viewWidth+xLoc;
		int end = (yLoc+ySize)*viewWidth;
		bufferDraw(start, end, red, green, blue);
	} else {
		for (int y=0;y<ySize;y++) {
			bufferDraw((yLoc+y)*viewWidth+xLoc,(yLoc+y)*viewWidth+xLoc+xSize, red, green, blue);
		}
	}
}
void Screen::drawPixel(int xLoc, int yLoc, uint32_t color) {
	if (xLoc>viewWidth||xLoc<0||yLoc>viewHeight||yLoc<0) {
		return;
	}
	uint8_t blue = color;
	uint8_t green = color >> 8;
	uint8_t red = color >> 16;
	int pixel = yLoc*viewWidth+xLoc;
	bufferDraw(pixel, pixel+1, red, green, blue);
}
