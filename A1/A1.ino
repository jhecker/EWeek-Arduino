//////////////////////////
////Required Libraries////
//////////////////////////

//COTS
#include <EnableInterrupt.h>

//Custom libraries
#include <Driving.h>
#include <RC.h>


////////////////
////Settings////
////////////////

//Driving
byte leftDirectionA = 8; //"clockwise" input
byte leftDirectionB = 7; //"counterclockwise" input
byte leftSpeedPin = 5; //PWM input
byte rightDirectionA = 9; //"clockwise" input
byte rightDirectionB = 4; //"counterclockwise" input
byte rightSpeedPin = 6; //PWM input

//Remote Control (RC)
byte linearPin = 2;
byte angularPin = 3;
float angularScaleFactor = 7;
byte robotDirectionPin = 19; //specifies which side of robot is "forward"; A5 as digital pin

//Robot Dimensions
float wheelBase = 0.285;
float wheelRadius = 0.06;
float maxVelocity = 1;


////////////////////////////
////Class Instantiations////
////////////////////////////

Driving drive = Driving(rightSpeedPin, rightDirectionA, rightDirectionB, leftSpeedPin, leftDirectionA, leftDirectionB);
RC rc = RC(linearPin, 1, angularPin, angularScaleFactor, robotDirectionPin, 1);


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
  //Check for RC commands
  if ((abs(rc.scaledCommand1()) > 0.2) || (abs(rc.scaledCommand2()) > 1.05)) {
    float left = (rc.scaledCommand1() + (rc.scaledCommand2() * (wheelBase / 2.0)));
    float right = (rc.scaledCommand1() - (rc.scaledCommand2() * (wheelBase / 2.0)));

    //normalize to [-maxVelocity, maxVelocity]
    float maxCmd = max(abs(left), abs(right));
    if (maxCmd > maxVelocity) {
      left = left / maxCmd * maxVelocity;
      right = right / maxCmd * maxVelocity;
    }
  
    if (left > 0) {
      left = pow(2.0, 4.4 * left + 3.6);
    }
    else if (left < 0) {
      left = -pow(2.0, 4.4 * abs(left) + 3.6);
    }
  
    if (right > 0) {
      right = pow(2.0, 4.4 * right + 3.6); 
    }
    else if (right < 0) {
      right = -pow(2.0, 4.4 * abs(right) + 3.6);
    }
    
    if (rc.scaledCommand3() > 0) {
      drive.drive((int)left, (int)right);
    }
    else {
      drive.drive(-(int)right, -(int)left);
    }
  }
  else {
    drive.stop();
  }
}
