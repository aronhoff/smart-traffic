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

		bool toSendInit = false;
		bool toSendInitSystemTime = false;
		bool toSendMotorUpdate = false;
		bool toSendRequestMotorState = false;
		bool toSendBatteryWarning = false;
		
		uint64_t motorUpdateTime;
		int8_t motorUpdateMotorSpeed;
		int8_t motorUpdateSteerPosition;

		int8_t requestMotorStateSpeed;
		int8_t requestMotorStateSteerPosition;

		static const int messageInit = 1;
		static const int messageSystemTime = 2;
		static const int messageMotorUpdate = 3;
		static const int messageMotorRequest = 4;
		static const int messageBattery = 5;

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
