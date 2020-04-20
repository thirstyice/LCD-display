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

#include "view.h"

#include <linux/fb.h>
#include <cstdint>

namespace LCDDisplay {
	class Screen: public View {
	public:
		Screen(const char* buffer = "/dev/fb1");
		~Screen();
		void drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color) override;
		void drawPixel(int xLoc, int yLoc, uint32_t color) override;
	private:
		void bufferDraw(int startPixel, int endPixel, uint8_t red, uint8_t green, uint8_t blue);
		unsigned char *fbPointer = 0;
		struct fb_var_screeninfo fbVarInfo;
		struct fb_fix_screeninfo fbFixInfo;
		long int fbSize = 0;
	};
}
