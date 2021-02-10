#ifndef __WEIGHT_SENSORS_CONTROLLER_H__
#define __WEIGHT_SENSORS_CONTROLLER_H__

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Config.h"
#include "HX711-multi.h"
#include "NodeMCUPins.h"



class WeightSensorsController
{
  private:
    byte *dataPins;
    int *scales;
    int offsets[SENSORS_COUNT];
    int precision;
    byte clockPin;
    void updateResult(uint8_t index);
    HX711MULTI* hx711;
  public:
    long int results[SENSORS_COUNT];
    uint8_t isChanged();
    WeightSensorsController(byte *dataPins, byte clockPin, int *scales, int precision);
    void calibrate(uint8_t index, int weight);
    void tare(uint8_t index);
};

#endif
