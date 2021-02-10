#include "LEDController.h"


LEDController::LEDController(byte *pins)
{
  ledPins = pins;
}
void LEDController::begin()
{
  for(int i = 0; i< SENSORS_COUNT; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    state[i] = LOW;
    digitalWrite(ledPins[i], state[i]);
  }
}
void LEDController::toggle(int i)
{
  state[i] = state[i]==LOW?HIGH:LOW;
  digitalWrite(ledPins[i], state[i]);
}
