#include "wireless.h"

Wireless::Wireless(HardwareSerial& serial) : mSerial(serial) {
	mSerial.begin(57600);
}

void Wireless::init() {
	writeHeader();
	write(messageInit);
	write(0);
	writeTime(0);
	writeFooter();
	
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

void Wireless::sendMessageIncorrect() {
	mToSendMessageIncorrect = true;
	while(available()) {
		if(available() < 2) mSerial.read();
		else {
			if(mSerial.read() != 0x00) continue;
			else if(mSerial.read() != 0xFE) continue;
		}
		

void Wireless::send() {
	// Header
	writeHeader();
	// Message
	if(mToSendInit) 
		write(messageInit);
	if(mToSendMotorUpdate) {
		write(messageMotorUpdate);
		writeTime(mMotorUpdateTime);
		write(mMotorUpdateMotorSpeed);
		write(mMotorUpdateSteerPosition);
	}
	if(mToSendRequestMotorState) {
		write(messageMotorRequest);
		write(mRequestMotorStateSpeed);
		write(mRequestMotorStateSteerPosition);
	}
	if(mToSendBatteryWarning)
		write(messageBattery);
	if(mToSendMessageIncorrect)
		write(messageMessageIncorrect);
	// End
	writeFooter();
	// Reset
	clean();
}

void Wireless::receive() {
	// Nothing to read?
	if(!available()) 
		return;
	//Header
	if(!readHeader())
		goto incorrectMessage; // !!! NOT BEGINNING OF MESSAGE
	// Message
	while(mSerial.peek() != 0xFE) {
		switch(mSerial.read()) {
			case messageInit:
				break;
			case messageMotorUpdate:
				uint64_t time = readTime();
				int8_t speed, steer;
				speed = mSerial.read();
				steer = mSerial.read();
				break;
			case messageMotorRequest:
				int8_t speed, steer;
				speed = mSerial.read();
				steer = mSerial.read();
				break;
			case messageBattery:
				break;
			case messageMessageIncorrect:
				break;
			case -1:
				break;
			default:
				goto incorrectMessage;
				break;
		}
	}
	// End
	if(!readFooter())
		goto incorrectMessage; // !!! MESSAGE NOT COMPLETE

	// More messages?
	receive();
	return;

incorrectMessage:
	sendMessageIncorrect();
	return;
}

void Wireless::clean() {
	mToSendInit = false;
	mToSendMotorUpdate = false;
	mToSendRequestMotorState = false;
	mToSendBatteryWarning = false;
	mToSendMessageIncorrect = false;
}
void Wireless::write(uint8_t x) {
	mSerial.write(x);
}
uint8_t Wireless::read() {
	return mSerial.read();
}
int Wireless::available() {
	return mSerial.available();
}
void Wireless::writeHeader() {
	mSerial.write(0x00);
	mSerial.write(0xFE);
}
void Wireless:writeFooter() {
	mSerial.write(0xFE);
	mSerial.write(0xFF);
}
bool Wireless::readHeader() {
	return mSerial.read() == 0x00
		&& mSerial.read() == 0xFE;
}
bool Wireless::readFooter() {
	return mSerial.read() == 0xFE
		&& mSerial.read() == 0xFF;
}
void Wireless::writeTime(uint64_t time) {
	for(int i = 4; i >= 0; i--) 
		write((uint8_t)(mMotorUpdateTime >> (i*8)) & 0xFF);
}
uint64_t Wireless::readTime() {
	uint64_t ret = 0;
	for(int i = 0; i < 5; i++) {
		ret |= read();
		ret << 8;
	}
}

void Wireless::setSetTime(voidLongCall call) {
	setTime = call;
}
