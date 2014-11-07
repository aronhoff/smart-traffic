#include "motor.h"

#include <Wire.h>
#include "common.h"

Motor::Motor() {
	Wire.begin();
}

void Motor::setSpeed(uint8_t speed) {
	mSpeed = speed;
	Wire.beginTransmission(Motor_Address);
	Wire.write(mSpeed);
	Wire.endTransmission();
}

uint8_t Motor::getSpeed() {
	return mSpeed;
}
