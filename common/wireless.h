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
		void writeHeader();
		void writeFooter();
		bool readHeader();
		bool readFooter();
		void writeTime(uint64_t);
		uint64_t readTime();
		void write(uint8_t x);
		uint8_t read();

		bool mToSendInit = false;
		bool mToSendInitSystemTime = false;
		bool mToSendMotorUpdate = false;
		bool mToSendRequestMotorState = false;
		bool mToSendBatteryWarning = false;
		bool mToSendMessageIncorrect = false;

		voidLongCall setTime;

		// Only using 5 bytes for the time!
		uint64_t mMotorUpdateTime;
		int8_t mMotorUpdateMotorSpeed;
		int8_t mMotorUpdateSteerPosition;

		int8_t mRequestMotorStateSpeed;
		int8_t mRequestMotorStateSteerPosition;

		static const int8_t messageInit = 1;
		static const int8_t messageMotorUpdate = 2;
		static const int8_t messageMotorRequest = 3;
		static const int8_t messageBattery = 4;
		static const int8_t messageMessageIncorrect = 5;

	public:
		Wireless(HardwareSerial& serial);

		int available();

		// Server -> car	Place car on starting position
		void init();
		// Server -> car	Initialize system time
		void initSystemTime();
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
		// Both				Message not received correctly
		void sendMessageIncorrect();
		// Both				Sends outstanding data
		void send();

		// Both				Receive outstanding data
		void receive();

		typedef void (*voidCall)();
		typedef void (*voidLongCall)(uint64_t);
		void setSetTime(voidLongCall call);
};
#endif
