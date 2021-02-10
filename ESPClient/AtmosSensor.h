#ifndef __ATMOS_SENSORS_CONTROLLER_H__
#define __ATMOS_SENSORS_CONTROLLER_H__

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "NodeMCUPins.h"
#include "Config.h"
#include <Wire.h>
#include <Adafruit_BME280.h>




class AtmosSensorController
{
  private:
    uint8_t sck, sda;
    TwoWire *i2c;
    Adafruit_BME280 *sensor;
    float tPrec;
    float hPrec;
    float pPrec;
    byte sensorAddr;
  public:
    float lastT;
    float lastP;
    float lastH;
    bool isChanged();
    void begin();
    AtmosSensorController(byte sdaPin, byte sckPin, byte addr, float tempPrecision, float humiPrecision, float presPrecision);
};

#endif
