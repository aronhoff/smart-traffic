#include <Servo.h>
#include "common/common.h"
#include "motor.h"
#include "common/wireless.h"

Wireless wireless(Serial, &getTime, &setTime);
uint64_t timeOffset = 0;
Motor motor;
/* Current servo range:
 *   60 degrees  - full left
 *   90 degrees  - centre
 *   120 degrees - full right
 */
Servo servo;

void setup() {
	//Initialize servo
	servo.attach(3);
	// Calibrate servo
	servo.write(90);
	delay(5000);

	//Initialize wireless
	/*wireless.begin();
	wireless.init();*/

	//Initialize TWI for motor control
	motor = Motor();
	motor.init();
	
	//Set
	motor.setSpeed(0);
	servo.write(60);
}

void loop() {
	//Check for new messages and receive them
	// wireless.update();
	//Steer and control motor
	//Send messages
}

void setTime(uint64_t t) {
	timeOffset = t - micros();
}

// TODO Handle micros() overflow (32-bit unsigned long)
uint64_t getTime() {
	return micros() + timeOffset;
}
