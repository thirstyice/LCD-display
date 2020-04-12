#include "buffer.h"

using namespace LCDDisplay;

FrameBuffer::FrameBuffer(const char *fileName) {
	int fbDevice=0;
	// Open the file for reading and writing
	fbDevice = open(fileName, O_RDWR);
	if (!fbDevice) {
		throw("Error: cannot open framebuffer device %s\n", fileName);
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
#ifdef DEBUG
	printf("The framebuffer device %s was opened successfully.\n", fileName);
	printf("Start + size: %i + %i\n",(int)fbPointer,fbSize);
#endif // DEBUG
	// cleanup
	close(fbDevice);
}
FrameBuffer::~FrameBuffer(){
	munmap(fbPointer, fbSize);
}
void FrameBuffer::draw(int startPixel, int endPixel, uint8_t red, uint8_t green, uint8_t blue) {
	if (startPixel > endPixel) {
		// Allow drawing reversed lines
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
