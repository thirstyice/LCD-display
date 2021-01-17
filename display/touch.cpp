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

#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <errno.h>

#include <libevdev-1.0/libevdev/libevdev.h>

#include "touch.h"

// this gets changed to be in the home folder if we compile without root
#ifndef CONF_LOC
	#define CONF_LOC "/etc/spi-display.conf"
#endif

using namespace LCDDisplay;

Touchscreen::Touchscreen(Screen* tmpScreen, const char * device) {
	screen=tmpScreen;
	calibrating=false;

	// load config
	confFile.open(CONF_LOC, std::ios::in | std::ios::out);
	if (confFile.is_open()) {
		// Setup options from file
		std::istream& conf = confFile;
		std::string line;
		while (std::getline(conf, line)) {
			std::istringstream lineStream (line);
			std::string key;
			if( std::getline(lineStream, key, '=') ) {
				std::string value;
				if( std::getline(lineStream, value) ) {
					// update variables
					if (key=="touchZeroX") {
						touchZeroX = std::stoi(value);
					} else if (key=="touchZeroY") {
						touchZeroY = std::stoi(value);
					} else if (key=="touchWidth") {
						touchWidth = std::stoi(value);
					} else if (key=="touchHeight") {
						touchHeight = std::stoi(value);
					} else if (key=="dragThresh") {
						dragThresh = std::stoi(value);
					} else if (key=="holdThresh") {
						holdThresh = std::stoi(value);
					} else if (key=="touchFlipXY") {
						// assert(value=="true"||value=="false");
						touchFlipXY=(value=="true");
					} else {
						std::cout << "ConfFile: unrecognized key:" << key << '\n';
					}
				}
			}
		}
	}

	fileDescriptor = open(device, O_RDONLY|O_NONBLOCK);
	evReciever = libevdev_new_from_fd(fileDescriptor, &evDevice);
	if (evReciever < 0) {
		std::cerr << "Failed to init libevdev device" << fileDescriptor << "\n";
		return;
	}
	listenerThread = new std::thread(&Touchscreen::listenForEvents, this);

	if (!confFile.is_open()) {
		confFile.open(CONF_LOC, std::ios::out);
		if (!confFile.is_open()) {
			std::cerr << "Conf file could not be opened or created; all configuration will be temporary" << '\n';
		}
		calibrate();
	}
}
Touchscreen::~Touchscreen() {
	close(fileDescriptor);
	confFile.close();
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
	// TODO if holding, send hold. If draging, send drag end. Otherwise send click
	touchX=-1;
	touchY=-1;
	initialTouchX=-1;
	initialTouchY=-1;
	initialTouchTime=std::chrono::steady_clock::time_point::min();
}
void Touchscreen::recievedTouchDown() {
	printf("Touch at: %i,%i\n", initialTouchX, initialTouchY);
	if (calibrating==true) {
		std::lock_guard<std::mutex> lk(caliMutex);
		caliWaitingForTouch=false;
		caliConditionVariable.notify_one();

		// return;
	}
	initialTouch=false;
}
void Touchscreen::recievedMove() {
	// TODO if the move triggers a drag, cancel the initialTouchTime so we stop listening for a hold
	printf("Move to: %i,%i\n", touchX, touchY);
}
void Touchscreen::recievedHold() {
	// TODO draw indicator rectangle to screen, don't send hold event untill touchUp
	printf("Recieved hold\n");
}
void Touchscreen::calibrate() {
	calibrating=true;
	int screenPointXY[2] = {50,50};
	int screenPointX[2] = {screen->viewWidth-50, 50};
	int screenPointY[2] = {50,screen->viewHeight-50};
	int touchPointXY[2];
	int touchPointX[2];
	int touchPointY[2];
	// TODO: disable screen updates from other sources while calibrating

	calibratePoint(screenPointXY, touchPointXY);
	calibratePoint(screenPointX, touchPointX);
	calibratePoint(screenPointY, touchPointY);

	if ((std::abs(touchPointXY[0]-touchPointX[0])<std::abs(touchPointXY[1]-touchPointX[1]))&&
			(std::abs(touchPointXY[1]-touchPointY[1])<std::abs(touchPointXY[0]-touchPointY[0]))) {
		touchFlipXY=true;
	}

	float dotsPerPixelX= (float)(touchPointX[touchFlipXY]-touchPointXY[touchFlipXY])/
			(float)(screenPointX[0]-screenPointXY[0]);
	float dotsPerPixelY= (float)(touchPointY[!touchFlipXY]-touchPointXY[!touchFlipXY])/
			(float)(screenPointY[1]-screenPointXY[1]);

	touchZeroX=touchPointXY[touchFlipXY]-(screenPointXY[0]*dotsPerPixelX);
	touchZeroY=touchPointXY[!touchFlipXY]-(screenPointXY[1]*dotsPerPixelY);
	touchWidth=touchPointX[touchFlipXY]+((screen->viewWidth-screenPointX[0])*dotsPerPixelX);
	touchHeight=touchPointY[!touchFlipXY]+((screen->viewHeight-screenPointY[1])*dotsPerPixelY);

	calibrating=false;
	screen->draw();
	if (confFile.is_open()) {
		//write variables to file
	}
}
void Touchscreen::calibratePoint(int* screenLocation, int* touchPoint) {
	screen->drawRect(0,0,screen->viewWidth,screen->viewHeight,0x000000);
	screen->drawRect(screenLocation[0]-5,screenLocation[1]-5,10,10,0x444444);
	screen->drawRect(screenLocation[0]-5,screenLocation[1],10,1,0xffffff);
	screen->drawRect(screenLocation[0],screenLocation[1]-5,1,10,0xffffff);

	caliWaitingForTouch=true;
	std::unique_lock<std::mutex> lk(caliMutex);
	// Can't use wait(lock, predi) for a non-static variable
	while (caliWaitingForTouch==true) {
		caliConditionVariable.wait(lk);
	}
	touchPoint[0]=initialTouchX;
	touchPoint[1]=initialTouchY;
	// TODO release touch event
}
