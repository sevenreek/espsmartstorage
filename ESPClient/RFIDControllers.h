#ifndef __RFID_SENSORS_CONTROLLER_H__
#define __RFID_SENSORS_CONTROLLER_H__

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Config.h"
#include <SPI.h>
#include <MFRC522.h>
#include "NodeMCUPins.h"
void dump_byte_array(byte *buffer, uint8_t bufferSize);


class RFIDSensorsController
{
  private:
    MFRC522* mfrc522[SENSORS_COUNT];
  public:
    byte lastID[SENSORS_COUNT][RFID_BYTE_SIZE];
    uint8_t noCardLastRead;
    uint8_t getLastID(int index, byte** result);
    void begin();
    uint8_t isChanged();
    RFIDSensorsController(byte *ssPins, byte *rstPins);
};

#endif
