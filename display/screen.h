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
