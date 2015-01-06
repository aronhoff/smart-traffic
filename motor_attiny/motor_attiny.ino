#include <TinyWireS.h>
#include <CustomStepper.h>

const uint8_t I2C_SLAVE_ADDRESS = 0x4;

CustomStepper stepper(0, 1, 2, 3, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, STOP);
bool stopped = true;

void setup() { 
   stepper.setRPM(12);
   stepper.setSPR(4075.7728395);
   stepper.setDirection(STOP);
   TinyWireS.begin(I2C_SLAVE_ADDRESS);                // join i2c bus with address #4	
}

void loop() {
	if (stepper.isDone() && !stopped) {
		//this will rotate until you stop it with another comand or set the direction to STOP
		stepper.rotate(1);
	}
	if(TinyWireS.available()) {
		uint8_t x = TinyWireS.receive();
		switch(x) {
			case 0:
				stopped = false;
				stepper.setDirection(CW);
				break;
			case 1: 
				stopped = false;
				stepper.setDirection(CCW);
				break;
			case 2:
				stopped = true;
				stepper.setDirection(STOP);
				break;
		}
		if(x != 2)
			stepper.rotate(1);
	}
	stepper.run();
}
