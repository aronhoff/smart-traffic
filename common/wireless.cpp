#include "wireless.h"

Wireless::Wireless(HardwareSerial& serial, ulongCall timeGetter, voidUlongCall timeSetter) : mSerial(serial),
		 mTimeGetter(timeGetter),
		 mTimeSetter(timeSetter)
{}

void Wireless::begin() {
	mSerial.begin(57600);
}

void Wireless::init() {
	// Do something?
	// Might be needed later
}

void Wireless::syncTime() {
	send(CTimeQuery, NULL, 0);
}

void Wireless::sendMotorUpdate(uint64_t time, int8_t speed, int8_t steer) {
	uint8_t a[7] = {};
	for(int i = 4; i >= 0; i--) {
		a[4-i] = ((time>>(i*8))&0xFF);
	}
	a[5] = speed;
	a[6] = steer;
	send(CMotorUpdate, a, 7);
}

void Wireless::requestMotorState(int8_t speed, int8_t steer) {
	uint8_t a[2] = {(uint8_t)speed, (uint8_t)steer};
	send(CMotorRequest, a, 2);
}

void Wireless::sendBatteryWarning() {
	send(CBatteryWarn, NULL, 0);
}

void Wireless::update() {
	receive();
	resend();
}

void Wireless::send(uint8_t command, uint8_t* payload, uint8_t length) {
	mMessageNum++;
	mMessageNum %= StoredMessages;
	while(mReceiveOK ^ (1<<mMessageNum)) {				// Message was not received!
		mMessageNum++; mMessageNum %= StoredMessages;
	}
	mCommands[mMessageNum] = command;
	mPayloadLengths[mMessageNum] = length;
	for(int i = 0; i < StoredMessageLength; i++) {
		if(i < length) mPayloads[mMessageNum][i] = payload[i];
		else mPayloads[mMessageNum][i] = 0;
	}
	mReceiveOK &= ~(1<<mMessageNum);
	sendNum(mMessageNum);
}

void Wireless::sendNum(uint8_t num) {
	mSendTime[num] = getTime();
	uint8_t parity = 0;
	mSerial.print("I");		// ID		
	printHex(num);
	parity ^= num;
	mSerial.print("C");		// Command
	printHex(mCommands[num]);
	parity ^= mCommands[num];
	mSerial.print("P");		// Payload
	for(int i = 0; i < mPayloadLengths[num]; i++) {
		printHex(mPayloads[num][i]);
		parity ^= mPayloads[num][i];
	}
	mSerial.print("Q");		// Parity
	printHex(parity);
	mSerial.print("\n");	// Newline
}

void Wireless::receive() {
	while(mSerial.available()) {
		mBuffer[mBufferPos] = mSerial.read();
		if(mBuffer[mBufferPos] == '\n') {
			parse();
			cleanBuffer();
		} else {
			mBufferPos++;
		}
	}
}

void Wireless::resend() {
	for(uint8_t i = 0; i < StoredMessages; i++) {
		if(!(mReceiveOK & (1 << i)) && getTime() - mSendTime[i] > Timeout) {
			sendNum(i);
		}
	}
}

void Wireless::onTimeQuery() {
	uint16_t timeout = 50000; // Microseconds
	send(CTimeResponse, NULL, 0);
	mReceiveOK |= (1 << mMessageNum); // Do not retry
	uint64_t time = getTime();
	while((!mSerial.available() || mSerial.read() != 'X')
	   && getTime()-time < timeout) {} // Deletes further messages!! (they will be re-sent) Better way?
	if(getTime()-time < timeout) return;			// Bad (timeout). Do sth?
	mSerial.print("X");
	// Now in sync
	time = getTime();
	while(!mSerial.available() && getTime()-time < timeout) {}
	if(getTime()-time < timeout) return;			// Bad (timeout). Do sth?
	time = getTime();
	for(uint8_t i = 4; i >= 0; i--)
		printHex((time >> (i*8)) & 0xFF);
	mSerial.read(); // Discard an X
}

void Wireless::onTimeResponse() {
	uint16_t timeout = 50000; // Microseconds
	mSerial.print("X");
	uint64_t time = getTime();
	uint64_t t1, t2;
	while(!mSerial.available() && getTime()-time < timeout) {}
	if(getTime()-time < timeout) return;			// Bad (timeout). Do sth?
	mSerial.read(); // Discard X
	// Now in sync
	t1 = getTime(); // Record initial time
	mSerial.print("X");
	time = getTime();
	while(!mSerial.available() && getTime()-time < timeout) {}
	if(getTime()-time < timeout) return;			// Bad (timeout). Do sth?
	t2 = getTime(); // Record final time
	// Read the time sent
	while(mSerial.available() < 6 && getTime()-time < timeout) {}
	if(getTime()-time < timeout) return;			// Bad (timeout). Do sth?
	mSerial.read(); // Discard X
	time = 0;
	for(uint8_t i = 0; i < 5; i++) {
		time <<= 8;
		time &= mSerial.read();
	}
	time += (t2-t1)/2;
	// Update send times of other messages
	int64_t difference = time - getTime();
	for(uint8_t i = 0; i < StoredMessages; i++)
		mSendTime[i] += difference;
	// I'm trusting no parity check is needed. Should I?
	// Use correction for the time it takes to get the time sent??
	setTime(time);
}

void Wireless::parse() {
	uint8_t pos = 0;
	// ID
	while(mBuffer[pos] != 'I') if(mBufferPos == pos) return; // Bad stuff
	pos++;
	uint8_t parity = 0;
	uint8_t id = parseHex(pos);
	parity ^= id;
	pos += 2;
	// Command
	while(mBuffer[pos] != 'C') if(mBufferPos == pos) return; // More bad stuff
	pos++;
	uint8_t command = parseHex(pos);
	parity ^= command;
	pos += 2;
	// Payload
	while(mBuffer[pos] != 'P') if(mBufferPos == pos) return; // Very bad stuff
	pos++;
	switch(command) {
		case CTimeQuery: {
			onTimeQuery();
			break;
		}
		case CTimeResponse: {
			onTimeResponse();
			break;
		}
		case CConfirmReceive: {
			mReceiveOK |= (1 << id);
			break;
		}
		case CMotorUpdate: {
			uint8_t motor, steer;
			uint64_t time = 0;
			for(uint8_t i = 0; i < 5; i++) {
				time <<= 8;
				time |= parseHex(pos);
				pos += 2;
			}
			motor = parseHex(pos); pos += 2;
			steer = parseHex(pos); pos += 2;
			// Do stuff
			break;
		}
		case CMotorRequest: {
			uint8_t motor, steer;
			motor = parseHex(pos); pos += 2;
			steer = parseHex(pos); pos += 2;
			// Do stuff
			break;
		}
		case CBatteryWarn: {
			// Do stuff
			break;
		}
	}
	// Parity
	while(mBuffer[pos] != 'Q') if(mBufferPos == pos) return; // Really bad stuff
	pos++;
	uint8_t par = parseHex(pos);
	if(par != parity) 								 return; // Serious bad stuff
	confirmMessage(id);										 // Good stuff
}

// Cannot use sendNum(), it would need the message in the buffer
void Wireless::confirmMessage(uint8_t id) { 
	mSerial.print("I");
	printHex(id);
	mSerial.print("C");
	printHex(CConfirmReceive);
	mSerial.print("P");
	mSerial.print("Q");
	printHex(id^CConfirmReceive);
	mSerial.print("\n");
}

void Wireless::printHex(uint8_t x) {
	mSerial.print(x >> 4, HEX);
	mSerial.print(x & 0xF, HEX);
}

uint8_t Wireless::parseHex(uint8_t pos) {
	uint8_t ret = 0, c;
	for(int i = 0; i < 2; i++) {
		c = mBuffer[pos+i];
		if(c >= '0' && c <= '9') c -= '0';
		else c += 10 - 'A';
		ret |= (c << (i*4));
	}
	return ret;
}

void Wireless::cleanBuffer() {
	for(uint8_t i = 0; i < BufferSize; i++) {
		mBuffer[i] = 0;
	}
	mBufferPos = 0;
}

uint64_t Wireless::getTime() {
	return mTimeGetter();
}

void Wireless::setTime(uint64_t time) {
	mTimeSetter(time);
}
