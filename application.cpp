#include "application.h"

using namespace LCDDisplay;

Application::Application() {
	buffer = new FrameBuffer("/dev/fb1");
	screen = new Screen(buffer);
}
Application::~Application() {
	delete screen;
	delete buffer;
}
