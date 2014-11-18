#include "wireless.h"

Wireless::Wireless(HardwareSerial& serial) : mSerial(serial) {
	mSerial.begin(57600);
}

void Wireless::init() {
	
}

void Wireless::initSystemTime() {
	
}

void Wireless::sendMotorUpdate(uint64_t time, int8_t motor, int8_t speed) {
	
}

void requestMotorState(int8_t motor, int8_t steer) {
	
}

void Wireless::sendBatteryWarning() {
	
}

void Wireless::send() {
	
}

void Wireless::receive() {
	
}
