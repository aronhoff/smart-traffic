#include <Arduino.h>
#include <TinyWireS.h>
#include <CustomStepper.h>
#include "common/common.h"

CustomStepper stepper(0, 1, 2, 3, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CCW);

void setup() { 
   stepper.setRPM(12);
   stepper.setSPR(4075.7728395);
   stepper.setDirection(CCW);
   TinyWireS.begin(I2C_SLAVE_ADDRESS);                // join i2c bus with address #4	
}

void loop() {
	if (stepper.isDone()) {
		//this will rotate until you stop it with another comand or set the direction to STOP
		stepper.rotate(1);
	}
	if(TinyWireS.available()) {
		uint8_t x = TinyWireS.receive();
		switch(x) {
			case 0:
				stepper.setDirection(CW);
				break;
			case 1: 
				stepper.setDirection(CCW);
				break;
			case 2:
				stepper.setDirection(STOP);
				break;
		}
		if(x != 2)
			stepper.rotate(1);
	}
	stepper.run();
}
