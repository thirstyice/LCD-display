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

#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>

#include <libevdev-1.0/libevdev/libevdev.h>

#include "touch.h"

using namespace LCDDisplay;

Touchscreen::Touchscreen(Screen* screen, const char * device) {
	fileDescriptor = open(device, O_RDONLY|O_NONBLOCK);
	evReciever = libevdev_new_from_fd(fileDescriptor, &evDevice);
	if (evReciever < 0) {
		std::cerr << "Failed to init libevdev device" << fileDescriptor << "\n";
		return;
	}
	listenerThread = new std::thread(&Touchscreen::listenForEvents, this);
}
Touchscreen::~Touchscreen() {
	close(fileDescriptor);
}
void Touchscreen::listenForEvents() {
	do {
		struct input_event ev;
		evReciever = libevdev_next_event(evDevice, LIBEVDEV_READ_FLAG_NORMAL, &ev);
		if (evReciever == 0) {
			#ifdef DEBUG
			printf("Event recieved: %s %s %d\n",
				libevdev_event_type_get_name(ev.type),
				libevdev_event_code_get_name(ev.type, ev.code),
				ev.value);
			#endif // DEBUG
			eventRecieved(ev.type, ev.code, ev.value);
		}
		if (initialTouchTime!=std::chrono::steady_clock::time_point::min()) {
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-initialTouchTime).count()>holdThresh) {
				recievedHold();
				initialTouchTime=std::chrono::steady_clock::time_point::min();
				// Now that we know we're holding, don't care when the initial touch happened
			}
		}
	} while (evReciever == 1 || evReciever == 0 || evReciever == -EAGAIN);
}
void Touchscreen::eventRecieved(unsigned int type, unsigned int code, int value) {
	switch (type) {
		case EV_KEY:
			if (code==BTN_TOUCH) {
				if (value==0) {
					recievedTouchUp();
				} else {
					initialTouch=true;
					initialTouchTime=std::chrono::steady_clock::now();
				}
			}
			break;
		case EV_ABS:
			switch (code) {
				case ABS_X:
					touchX=value;
					if (initialTouch) {
						initialTouchX=value;
						if (initialTouchY>=0) {
							recievedTouchDown();
							initialTouch=false;
						}
					} else {
						recievedMove();
					}
					break;
				case ABS_Y:
					touchY=value;
					if (initialTouch) {
						initialTouchY=value;
						if (initialTouchX>=0) {
							recievedTouchDown();
							initialTouch=false;
						}
					} else {
						recievedMove();
					}
					break;
			}
			break;
	}
}
void Touchscreen::recievedTouchUp() {
	touchX=-1;
	touchY=-1;
	initialTouchX=-1;
	initialTouchY=-1;
	initialTouchTime=std::chrono::steady_clock::time_point::min();
}
void Touchscreen::recievedTouchDown() {
	printf("Touch at: %i,%i\n", initialTouchX, initialTouchY);
}
void Touchscreen::recievedMove() {
	printf("Move to: %i,%i\n", touchX, touchY);
}
void Touchscreen::recievedHold() {
	printf("Recieved hold\n");
}
void Touchscreen::calibrate() {

}
