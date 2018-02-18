//////////////////////////
////Required Libraries////
//////////////////////////

//COTS
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h> 


////////////////
////Settings////
////////////////

//RFID
byte resetPin = 16; //A2 as digital pin
byte slaveSelectPin1 = 10;
byte slaveSelectPin2 = 17; //A3 as digital pin
MFRC522::MIFARE_Key key = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};


////////////////////////////
////Class Instantiations////
////////////////////////////

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 2, 16);
MFRC522 rfid1 = MFRC522(slaveSelectPin1, resetPin);
MFRC522 rfid2 = MFRC522(slaveSelectPin2, resetPin);


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
