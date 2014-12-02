#include "motor.h"
#include "common/common.h"
#include "common/wireless.h"

Wireless wireless(Serial, &getTime, &setTime);
uint64_t timeOffset = 0;
Motor motor;

void setup() {
	//Initialize wireless
	wireless.begin();
	wireless.init();
	//Initialize servo
	//Initialize TWI for motor control
	motor.init();
	//Initialize accelerometer
}

void loop() {
	//Check for new messages and receive them
	wireless.update();
	//Update position
	//Update track
	//Steer and control motor
	//Read accelerometer
	//Send messages
}

void setTime(uint64_t t) {
	timeOffset = t - micros();
}

// TODO Handle micros() overflow (32-bit unsigned long)
uint64_t getTime() {
	return micros() + timeOffset;
}
