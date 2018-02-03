#include <Crane.h>

// 8-bit bus after the 74HC595 shift register (not Arduino pins)
// Used to set direction of bridge driver.
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

//Constructor args are pins for shift register, and the arm and winch speeds
Crane::Crane(byte latchPin, byte clockPin, byte enablePin, byte dataPin, byte armSpeedPin, byte winchSpeedPin) {
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(armSpeedPin, OUTPUT);
    pinMode(winchSpeedPin, OUTPUT);
    digitalWrite(latchPin, LOW);
    digitalWrite(clockPin, LOW);
    digitalWrite(enablePin, LOW);
    digitalWrite(dataPin, LOW);

    _latchPin = latchPin;
    _clockPin = clockPin;
    _enablePin = enablePin;
    _dataPin = dataPin;
    _armSpeedPin = armSpeedPin;
    _winchSpeedPin = winchSpeedPin;

    Serial.begin(9600);
    switch (armSpeedPin) {
	case 11: //M1
	    _armDirectionA = MOTOR1_A;
            _armDirectionB = MOTOR1_B;
            break;
        case 3: //M2
	    _armDirectionA = MOTOR2_A;
            _armDirectionB = MOTOR2_B;
            break;
        case 6: //M3
	    _armDirectionA = MOTOR3_A;
            _armDirectionB = MOTOR3_B;
            break;
        case 5: //M4
	    _armDirectionA = MOTOR4_A;
            _armDirectionB = MOTOR4_B;
            break;
	default:
	    Serial.println("Error: Invalid arm speed pin");
	    break;
    }

    switch (winchSpeedPin) {
	case 11: //M1
	    _winchDirectionA = MOTOR1_A;
            _winchDirectionB = MOTOR1_B;
            break;
        case 3: //M2
	    _winchDirectionA = MOTOR2_A;
            _winchDirectionB = MOTOR2_B;
            break;
        case 6: //M3
	    _winchDirectionA = MOTOR3_A;
            _winchDirectionB = MOTOR3_B;
            break;
        case 5: //M4
	    _winchDirectionA = MOTOR4_A;
            _winchDirectionB = MOTOR4_B;
            break;
	default:
	    Serial.println("Error: Invalid winch speed pin");
	    break;
    }
}

//Set arm direction and speed according to input
void Crane::spinArm(int speed) {
    if (speed >= 0) {
    	shiftWrite(_armDirectionA, LOW);
    	shiftWrite(_armDirectionB, HIGH);
    }
    else {
    	shiftWrite(_armDirectionA, HIGH);
    	shiftWrite(_armDirectionB, LOW);
    }

    //Set PWM values using speed magnitudes
    analogWrite(_armSpeedPin, abs(speed));
}

//Stop arm
void Crane::stopArm() {
    shiftWrite(_armDirectionA, LOW);
    shiftWrite(_armDirectionB, LOW);
}

//Set winch direction and speed according to input
void Crane::moveWinch(int speed) {
    if (speed >= 0) {
    	shiftWrite(_winchDirectionA, LOW);
    	shiftWrite(_winchDirectionB, HIGH);
    }
    else {
    	shiftWrite(_winchDirectionA, HIGH);
    	shiftWrite(_winchDirectionB, LOW);
    }

    //Set PWM values using speed magnitudes
    analogWrite(_winchSpeedPin, abs(speed));
}

//Stop winch
void Crane::stopWinch() {
    shiftWrite(_winchDirectionA, LOW);
    shiftWrite(_winchDirectionB, LOW);
}

//Set motor pins in shift register
void Crane::shiftWrite(byte pin, byte value) {
    static int latchValue = 0;

    bitWrite(latchValue, pin, value);
    shiftOut(_dataPin, _clockPin, MSBFIRST, latchValue);
    delayMicroseconds(5);
    digitalWrite(_latchPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_latchPin, LOW);
}
