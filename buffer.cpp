#include "buffer.h"

using namespace LCDDisplay;

FrameBuffer::FrameBuffer(const char *fileName) {
	int fbDevice=0;
	// Open the file for reading and writing
	fbDevice = open(fileName, O_RDWR);
	if (!fbDevice) {
		throw("Error: cannot open framebuffer device %s\n", fileName);
	}
	printf("The framebuffer device %s was opened successfully.\n", fileName);

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

	fbPointer = (char*)mmap(0,fbSize,PROT_READ | PROT_WRITE,MAP_SHARED,fbDevice, 0);
	if ((int)fbPointer == -1) {
		throw("Failed to mmap.\n");
	}
	// cleanup
	close(fbDevice);
}
FrameBuffer::~FrameBuffer(){
	munmap(fbPointer, fbSize);
}
void FrameBuffer::draw(int startPixel, int endPixel, int data) {
	void * start = fbPointer+(startPixel*fbVarInfo.bits_per_pixel/8);
	size_t length = (endPixel-startPixel)*fbVarInfo.bits_per_pixel/8;
	memset(start, data, length);
#ifdef DEBUG
	// Log the data being passed to memory
	unsigned char *p = (unsigned char*)start;
	size_t i;
	for (i=0; i < length; ++i) {
		printf("%02x\n", p[i]);
	}
#endif //DEBUG
}
