#include "application.h"

using namespace LCDDisplay;

Application::Application() {
	buffer = new FrameBuffer("/dev/fb1");
	mainView = new Screen(buffer);
}
Application::~Application() {
	delete mainView;
	delete buffer;
}
