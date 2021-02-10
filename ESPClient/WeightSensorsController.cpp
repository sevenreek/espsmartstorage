#include "WeightSensorsController.h"

WeightSensorsController::WeightSensorsController(byte *dataPins, byte clockPin, int *scales, int precision)
{
  this->scales = scales;
  this->precision = precision;
  this->hx711 = new HX711MULTI(SENSORS_COUNT, dataPins, clockPin);
}
uint8_t WeightSensorsController::isChanged()
{
  if(!hx711->is_ready()) return false;
  uint8_t change = 0b0;
  long int rawResults[SENSORS_COUNT];
  hx711->readRaw(rawResults);
  for(int i = 0; i<SENSORS_COUNT; i++)
  {
    
    long int result = (rawResults[i]-offsets[i])/scales[i];
    #ifdef ENABLE_SERIAL
      Serial.print(i);Serial.print(":");Serial.print(rawResults[i]);Serial.print("->");Serial.print(result);Serial.print(" ");
    #endif
      if(result>=0) 
      {
        if(abs(result-results[i])>precision)
        {
          results[i] = result;
          change |= (0b1<<i);
        }
      }
      else if(results[i] != 0)
      {
        results[i] = 0;
        change |= (0b1<<i);
      } 
  }
  Serial.println();
  return change;
}
void WeightSensorsController::tare(uint8_t index)
{
  if(index>=SENSORS_COUNT) return;
  
  long int rawResults[SENSORS_COUNT];
  hx711->readRaw(rawResults);
  long int movingAvg = rawResults[index];
  for(int i =0; i<TARE_MA_REPEAT; i++)
  {
    hx711->readRaw(rawResults);
    movingAvg += rawResults[index];
    movingAvg /= 2;
  }
  offsets[index] = movingAvg;
}
void WeightSensorsController::calibrate(uint8_t index, int weight)
{
  if(index>=SENSORS_COUNT) return;
  long int rawResults[SENSORS_COUNT];
  hx711->readRaw(rawResults);
  scales[index] = (rawResults[index]-offsets[index])/weight;
}
