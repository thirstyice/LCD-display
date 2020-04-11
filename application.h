#pragma once

#include "screen.h"

namespace LCDDisplay {
	class Application {
	private:
		/* data */
		FrameBuffer* buffer;
		Screen* screen;
	public:
		Application();
		virtual ~Application();
	};
} /* LCDDisplay */
