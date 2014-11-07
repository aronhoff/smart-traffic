//Controlling an attiny85 through the Wire library
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
	private:
		uint8_t mSpeed;
	public:
		Motor();
		void setSpeed(uint8_t speed);
		uint8_t getSpeed();
};
#endif
