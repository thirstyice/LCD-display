#pragma once

#include <cstdint>

#include "buffer.h"
#include "view.h"

namespace LCDDisplay {
	class Screen: public View {
	public:
		Screen(FrameBuffer* buffer);
		~Screen();
		void drawRect(int xLoc, int yLoc, int xSize, int ySize, uint32_t color) override;
		void drawPixel(int xLoc, int yLoc, uint32_t color) override;
	private:
		FrameBuffer* buffer;
	};
}
