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
byte robotDirectionPin = 15; //specifies which side of robot is "forward"; A1 as digital pin

//RFID
byte resetPin = 16; //A2 as digital pin
byte slaveSelectPin1 = 10;
byte slaveSelectPin2 = 17; //A3 as digital pin
MFRC522::MIFARE_Key key = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

//Robot Dimensions
float wheelBase = 0.285;
float wheelRadius = 0.06;
float maxVelocity = 1;


////////////////////////////
////Class Instantiations////
////////////////////////////

Driving drive = Driving(rightSpeedPin, rightDirectionA, rightDirectionB, leftSpeedPin, leftDirectionA, leftDirectionB);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 2, 16);
MFRC522 rfid1 = MFRC522(slaveSelectPin1, resetPin);
MFRC522 rfid2 = MFRC522(slaveSelectPin2, resetPin);
RC rc = RC(linearPin, 1, angularPin, angularScaleFactor, robotDirectionPin, 1);


/////////////
////Setup////
/////////////

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid1.PCD_Init();
  rfid2.PCD_Init();
  lcd.begin();   
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

  // Look for new RFID cards
  if (rfid1.PICC_IsNewCardPresent() && rfid1.PICC_ReadCardSerial()) {
    //Clear display
    lcd.clear();
    byte length = 18;
    byte msg[length];
    if (rfid1.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(rfid1.uid)) == MFRC522::STATUS_OK) {
      rfid1.MIFARE_Read(4, msg, &length);
      //Write information to LCD screen
      lcd.setCursor(0,0);
      lcd.printstr(msg);
    }
    if (rfid1.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(rfid1.uid)) == MFRC522::STATUS_OK) {
      rfid1.MIFARE_Read(1, msg, &length);
      //Write information to LCD screen
      lcd.setCursor(0,1);
      lcd.printstr(msg);
    }
    rfid1.PICC_HaltA();
    rfid1.PCD_StopCrypto1();
  }

  if (rfid2.PICC_IsNewCardPresent() && rfid2.PICC_ReadCardSerial()) {
    //Clear display
    lcd.clear();
    byte length = 18;
    byte msg[length];
    if (rfid2.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(rfid2.uid)) == MFRC522::STATUS_OK) {
      rfid2.MIFARE_Read(4, msg, &length);
      //Write information to LCD screen
      lcd.setCursor(0,0);
      lcd.printstr(msg);
    }
    if (rfid2.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(rfid2.uid)) == MFRC522::STATUS_OK) {
      rfid2.MIFARE_Read(1, msg, &length);
      //Write information to LCD screen
      lcd.setCursor(0,1);
      lcd.printstr(msg);
    }
    rfid2.PICC_HaltA();
    rfid2.PCD_StopCrypto1();
  }

}
