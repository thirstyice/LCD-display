#include "screen.h"
#include "buffer.h"

using namespace LCDDisplay;

int main(int argc, char const *argv[]) {
	FrameBuffer buffer("/dev/fb1");
	Screen screen(&buffer);
	screen.drawRect(100, 100, 200,200, 0xff0000);
	return 0;
}
