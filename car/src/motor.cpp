#include "motor.h"

#include <Wire.h>
#include "common/common.h"

int8_t mSpeed = 0;

void Motor::init() {
	Wire.begin();
}

void Motor::setSpeed(int8_t speed) {
	mSpeed = speed;
	Wire.beginTransmission(Motor_Address);
	Wire.write(mSpeed);
	Wire.endTransmission();
}

int8_t Motor::getSpeed() {
	return mSpeed;
}
