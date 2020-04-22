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
#include <chrono>
#include <thread>

#include <libevdev-1.0/libevdev/libevdev.h>

namespace LCDTouch {
	class Touchscreen {
	private:
		int fileDescriptor = 0;
		int evReciever = 1;
		struct libevdev* evDevice=0;
		int dragThresh = 10; // # of pixels before a touch becomes a drag
		int holdThresh = 250; // # of miliseconds before a click becomes a hold
		std::thread* listenerThread;
		void listenForEvents();
		void eventRecieved(unsigned int type, unsigned int code, int value);
		void recievedTouchDown();
		void recievedTouchUp();
		void recievedMove();
		void recievedHold();
		void updateState();
		bool initialTouch = false;
		int initialTouchX=-1;
		int initialTouchY=-1;
		std::chrono::steady_clock::time_point initialTouchTime = std::chrono::steady_clock::time_point::min();
		bool touchState = false;
		bool draggging = false;
		int touchX = -1;
		int touchY = -1;

		// Calibration info
		int touchZeroLocX;
		int touchZeroLocY;
		int touchWidth;
		int touchHeight;
	public:
		Touchscreen(const char * device = "/dev/input/event0");
		virtual ~Touchscreen();
		// void onTouchDownEventDo(void(*functionPointer)());
	};
} /* LCDTouch */
