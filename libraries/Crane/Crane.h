#ifndef Crane_h
#define Crane_h

#include "Arduino.h"

class Crane
{
	public:
		//Constructors
		Crane(byte latchPin, byte clockPin, byte enablePin, byte dataPin, byte armSpeedPin, byte winchSpeedPin);
		
		//Functions
		void spinArm(int speed);
                void stopArm();
		void moveWinch(int speed);
                void stopWinch();
		
	private:
                void shiftWrite(byte pin, byte value);
        	byte _latchPin, _clockPin, _enablePin, _dataPin, _armSpeedPin, _winchSpeedPin;
		byte _armDirectionA, _armDirectionB, _winchDirectionA, _winchDirectionB;
};

#endif
