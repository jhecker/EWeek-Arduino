//////////////////////////
////Required Libraries////
//////////////////////////

//COTS
#include <EnableInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h> 

//Custom libraries
#include <Driving.h>
#include <RC.h>

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

//RFID
byte slaveSelectPin = 10;
byte resetPin = 17; //A4 as digital pin

//Robot Dimensions
float wheelBase = 0.285;
float wheelRadius = 0.06;
float maxVelocity = 1;


////////////////////////////
////Class Instantiations////
////////////////////////////

Driving drive = Driving(rightSpeedPin, rightDirectionA, rightDirectionB, leftSpeedPin, leftDirectionA, leftDirectionB);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 2, 16);
MFRC522 rfid = MFRC522(slaveSelectPin, resetPin);
RC rc = RC(linearPin, 1, angularPin, angularScaleFactor);


/////////////
////Setup////
/////////////

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.begin();   
}


/////////////////
////Main Loop////
/////////////////

void loop()
{
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
    
    drive.drive((int)left, (int)right);
  }
  else {
    drive.stop();
  }
}
