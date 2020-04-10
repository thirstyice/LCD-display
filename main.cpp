#include "buffer.h"

using namespace LCDDisplay;

int main(int argc, char const *argv[]) {
	FrameBuffer buffer("/dev/fb1");
	buffer.draw(0,buffer.getXres()*buffer.getYres(),0x00, 0x00, 0xff);
	// buffer.draw(0,1,0xffffff);
	return 0;
}
