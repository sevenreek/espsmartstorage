#include "AtmosSensor.h"

bool AtmosSensorController::isChanged()
{
  bool anyChanged = false;
  float newT = sensor->readTemperature();
  if(abs(newT-lastT)>tPrec) {
    lastT = newT;
    anyChanged = true;
  }
  float newP = sensor->readPressure();
  if(abs(newP-lastP)>pPrec) {
    lastP = newP;
    anyChanged = true;
  }
  float newH = sensor->readHumidity();
  if(abs(newH-lastH)>hPrec) {
    lastH = newH;
    anyChanged = true;
  }
  return anyChanged;
}
void AtmosSensorController::begin()
{
  i2c->begin(sda, sck);
  sensor->begin(sensorAddr, i2c);
  delay(5);
}
AtmosSensorController::AtmosSensorController(byte sdaPin, byte sckPin, byte addr, float tempPrecision, float humiPrecision, float presPrecision)
{
  sda = sdaPin;
  sck = sckPin;
  tPrec = tempPrecision;
  hPrec = humiPrecision;
  pPrec = presPrecision;
  sensorAddr = addr;
  i2c = new TwoWire();
  sensor = new Adafruit_BME280();
}
