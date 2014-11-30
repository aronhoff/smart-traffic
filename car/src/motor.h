//Controlling an attiny85 through the Wire library
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
	private:
		int8_t mSpeed = 0;
		static bool isThereAnInstanceAlready; // Descriptive enough?
	public:
		void init();
		void setSpeed(int8_t speed);
		int8_t getSpeed();
};
#endif
