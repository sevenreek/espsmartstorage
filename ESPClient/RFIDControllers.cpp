#include "RFIDControllers.h"

uint8_t RFIDSensorsController::getLastID(int index, byte** result) 
{
    *result = lastID[index];
    return RFID_BYTE_SIZE;
}

RFIDSensorsController::RFIDSensorsController(byte *ssPins, byte *rstPins) 
{
  for(int i = 0; i<SENSORS_COUNT; i++)
  {
    this->mfrc522[i] = new MFRC522(ssPins[i], rstPins[i]);  
  }
}

uint8_t RFIDSensorsController::isChanged() 
{
  uint8_t result = 0b0;
  for(int i=0; i< SENSORS_COUNT; i++)
  {
    if (mfrc522[i]->PICC_IsNewCardPresent()) 
    {
      Serial.print(i);Serial.print(": ");Serial.println("New card");
      noCardLastRead &= ~(0b1 << i);
      if (mfrc522[i]->PICC_ReadCardSerial()) 
      {
        
        #ifdef ENABLE_SERIAL
        Serial.print(i);Serial.print(": ");Serial.println("Read serial");
        #endif
        if(memcmp(mfrc522[i]->uid.uidByte, lastID[i], RFID_BYTE_SIZE*sizeof(byte)) != 0)
        {
          #ifdef ENABLE_SERIAL
          //Serial.print("NEW:");Serial.print("uidByte="); dump_byte_array(mfrc522[i]->uid.uidByte,4);Serial.print("; ");Serial.print("lastID="); dump_byte_array(lastID,4);Serial.println();
          #endif
          result |= (0b1 << i);
          memcpy(lastID[i], mfrc522[i]->uid.uidByte, RFID_BYTE_SIZE*sizeof(byte));
        }
        
      }
    }
    else if((noCardLastRead & ((0b1)<<i)) && lastID[i][0] != 0 && lastID[i][1] != 0) // checks just 2 first bytes, extend to 4 if needed
    {
      // change occured to no card detected
      //Serial.print("NONE:");Serial.print("uidByte="); dump_byte_array(mfrc522[i]->uid.uidByte,4);Serial.print("; ");Serial.print("lastID="); dump_byte_array(lastID,4);Serial.println();
      result |= (0b1 << i);
      memset(lastID[i], 0, RFID_BYTE_SIZE*sizeof(byte));
    }
    else
    {
      // no card but could just be the module being bad
      noCardLastRead |= (0b1 << i);
    }
  }
  return result;
}
 

void RFIDSensorsController::begin()
{
  SPI.begin();      // Init SPI bus
  for(int i=0; i< SENSORS_COUNT; i++)
  {
    mfrc522[i]->PCD_Init();
    delay(4); 
  }
}

void dump_byte_array(byte *buffer, uint8_t bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
