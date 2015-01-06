//Controlling an attiny84 through the Wire library
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
	private:
		int8_t mSpeed = 0;
	public:
		void init();
		// Current configuration:
		// 0 - forward
		// 1 - backward
		// 2 - stop
		void setSpeed(int8_t speed);
		int8_t getSpeed();
};
#endif
