//Controlling an attiny85 through the Wire library
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
	private:
		static int8_t mSpeed;
	public:
		static void init();
		static void setSpeed(int8_t speed);
		static int8_t getSpeed();
};
#endif
