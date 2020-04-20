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
