#include "motor.h"

#include <Wire.h>
#include "common/common.h"

bool Motor::isThereAnInstanceAlready = false;

void Motor::init() {
	if(isThereAnInstanceAlready) {
		// DO NOT DO THIS!
		return;
	}
	isThereAnInstanceAlready = true;
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
