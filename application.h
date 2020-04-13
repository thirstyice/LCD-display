#pragma once

#include "screen.h"

namespace LCDDisplay {
	class Application {
	private:
		/* data */
		FrameBuffer* buffer;
	public:
		View* mainView;
		Application();
		virtual ~Application();
	};
} /* LCDDisplay */
