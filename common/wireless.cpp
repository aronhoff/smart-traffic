#include "wireless.h"

Wireless::Wireless(HardwareSerial& serial) : mSerial(serial) {
	mSerial.begin(57600);
}

void Wireless::init() {
	mToSendInit = true;
}

void Wireless::initSystemTime() {
	mToSendInitSystemTime = true;
}

void Wireless::sendMotorUpdate(uint64_t time, int8_t speed, int8_t steer) {
	mToSendMotorUpdate = true;
	mMotorUpdateTime = time;
	mMotorUpdateMotorSpeed = speed;
	mMotorUpdateSteerPosition = steer;
}

void Wireless::requestMotorState(int8_t speed, int8_t steer) {
	mToSendRequestMotorState = true;
	mRequestMotorStateSpeed = speed;
	mRequestMotorStateSteerPosition = steer;
}

void Wireless::sendBatteryWarning() {
	mToSendBatteryWarning = true;
}

void Wireless::send() {
	mSerial.write(0);
	if(mToSendInit) 
		mSerial.write(messageInit);
	if(mToSendInitSystemTime)
		mSerial.write(messageSystemTime);
	if(mToSendMotorUpdate) {
		mSerial.write(messageMotorUpdate);
		for(int i = 4; i >= 0; i++) 
			mSerial.write((uint8_t)(mMotorUpdateTime >> (i*8)) & 0xFF);
		mSerial.write(mMotorUpdateMotorSpeed);
		mSerial.write(mMotorUpdateSteerPosition);
	}
	if(mToSendRequestMotorState) {
		mSerial.write(messageMotorRequest);
		mSerial.write(mRequestMotorStateSpeed);
		mSerial.write(mRequestMotorStateSteerPosition);
	}
	if(mToSendBatteryWarning)
		mSerial.write(messageBattery);
	mSerial.write(0xFF);

	clean();
}

void Wireless::receive() {
	
}

void Wireless::clean() {
	mToSendInit = false;
	mToSendInitSystemTime = false;
	mToSendMotorUpdate = false;
	mToSendRequestMotorState = false;
	mToSendBatteryWarning = false;
}
