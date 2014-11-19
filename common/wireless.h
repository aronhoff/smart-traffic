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
	private:
		HardwareSerial& mSerial;

		void clean();

		bool mToSendInit = false;
		bool mToSendInitSystemTime = false;
		bool mToSendMotorUpdate = false;
		bool mToSendRequestMotorState = false;
		bool mToSendBatteryWarning = false;
		
		// Only using 5 bytes for the time!
		uint64_t mMotorUpdateTime;
		int8_t mMotorUpdateMotorSpeed;
		int8_t mMotorUpdateSteerPosition;

		int8_t mRequestMotorStateSpeed;
		int8_t mRequestMotorStateSteerPosition;

		static const int8_t messageInit = 1;
		static const int8_t messageSystemTime = 2;
		static const int8_t messageMotorUpdate = 3;
		static const int8_t messageMotorRequest = 4;
		static const int8_t messageBattery = 5;

	public:
		Wireless(HardwareSerial& serial);

		// Server -> car	Place car on starting position
		void init();
		// Server -> car	Initialize system time
		void initSystemTime();
		// Car -> server	Send new motor data
		void sendMotorUpdate(uint64_t time, int8_t motor, int8_t steer);
		// Server -> car	Request motor operation
		void requestMotorState(int8_t motor, int8_t steer);
		// Car -> server	Send low battery warning
		void sendBatteryWarning();
		// Both				Sends outstanding data
		void send();

		// Both				Receive outstanding data
		void receive();
};
#endif
