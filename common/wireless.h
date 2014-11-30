//Controlling HC-11 through Serial

#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>
#include "common.h"

// time sync
// obstacle
// position
// motor and steer
// route
// crossing permission
// pedestrian
// battery

class Wireless {
	public:
		Wireless(HardwareSerial& serial);
		void begin();

		// Car -> server	Synchronize system time
		void init();
		// Server -> car	Inform car about a new obstacle
//		void sendObstacle(/*position*/);
		// Server -> car	Inform car about obstacle removed
//		void removeObstacle(/*position*/);
		// Server -> car	Inform car about position sensor result
//		void updatePosition(/*time, position*/);
		// Car -> server	Send new motor data
		void sendMotorUpdate(uint64_t time, int8_t motor, int8_t steer);
		// Server -> car	Request motor operation
		void requestMotorState(int8_t motor, int8_t steer);
		// 
//!!!!!		void setRoute(/*routeID*/);
		// Server -> car	Signals a new pedestrian at position
//		void putPedestrian();
		// Server -> car	Pedestrian removed
//		void removePedestrian();
		// Server -> car	Give permission for the next crossing
//		void allowCrossing();
		// Car -> server	Send low battery warning
		void sendBatteryWarning();

		// Both				Receive outstanding data
		void receive();

		typedef void (*voidCall)();
		typedef void (*voidUlongCall)(uint64_t);
		typedef uint64_t (*ulongCall)();

		void setTimeSetter(voidUlongCall call);
		void setTimeGetter(ulongCall call);
		void setInit(voidCall call);

	private:
		HardwareSerial& mSerial;

		static const uint8_t StoredMessages = 8;
		static const uint8_t StoredMessageLength = 7; // Length of the longest possible message is currently 7
		static const uint8_t BufferSize = 20;
		
		static const uint8_t CTimeQuery = 0;
		static const uint8_t CTimeResponse = 1;
		static const uint8_t CConfirmReceive = 2;
		static const uint8_t CMotorUpdate = 3;
		static const uint8_t CMotorRequest = 4;
		static const uint8_t CBatteryWarn = 5;

		uint8_t mMessageNum = 0;
		uint8_t mCommands[StoredMessages];
		uint8_t mPayloads[StoredMessages][StoredMessageLength];
		uint8_t mPayloadLengths[StoredMessages];
		uint8_t mReceiveOK = ~0; // Use bitwise operiations!
		uint8_t mBuffer[BufferSize];
		uint8_t mBufferPos;

		// Send command with payload
		void send(uint8_t command, uint8_t* payload, uint8_t length);
		// Send message with message number
		void sendNum(uint8_t num);
		// Print byte on serial port in hexadecimal format
		void printHex(uint8_t x);
		// Read hexadecimal byte from buffer at pos
		uint8_t parseHex(uint8_t pos);
		// Clean buffer
		void cleanBuffer();
		// Parse message from the buffer
		void parse();
		// Send message confirmation
		void confirmMessage(uint8_t id);

		voidUlongCall mTimeSetter;
		ulongCall mTimeGetter;
		voidCall mInit;
};
#endif
