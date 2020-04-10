#pragma once

#include <cstdint>

#include "buffer.h"

namespace LCDDisplay {
	class Screen {
	public:
		Screen(FrameBuffer* initBuffer);
		~Screen();
		void drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color);
		uint32_t bgColor = 0x000000;
	private:
		FrameBuffer* buffer;
		uint32_t** screenArray;
	};
}
