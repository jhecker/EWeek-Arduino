//////////////////////////
////Required Libraries////
//////////////////////////

//COTS
#include <EnableInterrupt.h>

//Custom libraries
#include <Crane.h>
#include <RC.h>

//Crane
byte latchPin = 12;
byte clockPin = 4;
byte enablePin = 7;
byte dataPin = 8;
byte armSpeedPin = 11; //M1
byte winchSpeedPin = 3; //M2

//Remote Control (RC)
byte armPin = 9;
byte winchPin = 10;


////////////////////////////
////Class Instantiations////
////////////////////////////

Crane crane = Crane(latchPin, clockPin, enablePin, dataPin, armSpeedPin, winchSpeedPin);
RC rc = RC(armPin, 1, winchPin, 1);


/////////////
////Setup////
/////////////

void setup()
{
  Serial.begin(9600);
}


/////////////////
////Main Loop////
/////////////////

void loop()
{
  //We discretize these commands so the crane actuators always move at a fixed speed for simpler user interaction
  
  if (rc.scaledCommand1() > 0.2) {
    crane.spinArm(100);
  }
  else if (rc.scaledCommand1() < -0.2) {
    crane.spinArm(-100);
  }
  else {
    crane.stopArm();
  }
  
  if (rc.scaledCommand2() > 0.2) {
    crane.moveWinch(100);
  }
  else if (rc.scaledCommand2() < -0.2) {
    crane.moveWinch(-100);
  }
  else {
    crane.stopWinch();
  }
}
