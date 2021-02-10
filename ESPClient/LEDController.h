#ifndef __LED_SENSORS_CONTROLLER_H__
#define __LED_SENSORS_CONTROLLER_H__

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Config.h"
#include "NodeMCUPins.h"



class LEDController
{
  private:
    byte *ledPins;
    long blinkLastChange;
    byte state[SENSORS_COUNT];
  public:
    
    LEDController(byte *ledPins);
    void toggle(int i);
    void begin();
};

#endif
