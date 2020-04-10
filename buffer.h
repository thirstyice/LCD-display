#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstdint>

namespace LCDDisplay {
	class FrameBuffer {
	public:
		FrameBuffer(const char *fileName = "/dev/fb0"); //bufferId is X in /dev/fbX
		~FrameBuffer();
		void draw(int startPixel, int endPixel, uint8_t red, uint8_t green, uint8_t blue);
		int getXres() {return fbVarInfo.xres;};
		int getYres() {return fbVarInfo.yres;};
	private:
		unsigned char *fbPointer = 0;
		struct fb_var_screeninfo fbVarInfo;
		struct fb_fix_screeninfo fbFixInfo;
		long int fbSize = 0;
	};
}
