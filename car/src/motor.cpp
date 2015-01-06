#include "motor.h"

#include <Wire.h>
#include "common/common.h"

#define MOTOR_ADDRESS 0x4

void Motor::init() {
	Wire.begin();
}

void Motor::setSpeed(int8_t speed) {
	mSpeed = speed;
	Wire.beginTransmission(MOTOR_ADDRESS);
	Wire.write(mSpeed);
	Wire.endTransmission();
}

int8_t Motor::getSpeed() {
	return mSpeed;
}
