#include <EspMQTTClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



#define SEA_PRESSURE 1013.25
#define SHELF_ID "1"
#define PRECISION_T 0.1f
#define PRECISION_P 0.1f
#define PRECISION_H 1.0f
#define DELAY 1*1000

EspMQTTClient client(
  SSID,
  PASSWORD,
  MQTT_IP,  
  SHELF_ID "_ATMOS",     
  1883              
);

Adafruit_BME280 atmosSensor;
float temperature = 0;
float humidity = 0;
float pressure = 0;

void setup() {
  Serial.begin(9600);   
  
  while(!Serial);
  Serial.println(F("Starting"));
  client.enableDebuggingMessages(); 
  if(!atmosSensor.begin(0x76)) {
    Serial.println("Failed to find sensor");
    while(true);   // watchdog will reboot
  }
  atmosSensor.setSampling(Adafruit_BME280::MODE_FORCED,
                          Adafruit_BME280::SAMPLING_X1, // temperature
                          Adafruit_BME280::SAMPLING_X1, // pressure
                          Adafruit_BME280::SAMPLING_X1, // humidity
                          Adafruit_BME280::FILTER_OFF   );
  Serial.println(F("Running"));
}
long int lastMeasurement;
void loop() {
  client.loop();
  if(millis()>lastMeasurement+DELAY)
  {
    lastMeasurement = millis();
    atmosSensor.takeForcedMeasurement();
    bool changed = false;
    float newTemperature = atmosSensor.readTemperature();
    if(abs(newTemperature-temperature)>PRECISION_T){
      changed = true;
      temperature = newTemperature;
    }
    float newHumidity = atmosSensor.readHumidity();
    if(abs(newHumidity-humidity)>PRECISION_H){ 
      changed = true;
      humidity = newHumidity;
    }
    float newPressure = atmosSensor.readPressure() / 100.0f;
    if(abs(newPressure-pressure)>PRECISION_P){
      changed = true;
      pressure = newPressure;
    }
    if(changed)
    {
      String data = "{\"t\":"; data += temperature; data +=", \"p\":"; data += pressure; data +=", \"h\":"; data+= humidity; data += "}";
      client.publish("shelves/" SHELF_ID "/atmos", data);
    }
    String data = "{'t':"; data += temperature; data +=", 'p':"; data += pressure; data +=", 'h':"; data+= humidity; data += "}";
    Serial.println(data);
  }
}

void onConnectionEstablished()
{
  client.subscribe("shelves/" SHELF_ID "/getall" , [](const String & payload) {
    String data = "{\"t\":"; data += temperature; data +=", \"p\":"; data += pressure; data +=", \"h\":"; data+= humidity; data += "}";
    client.publish("shelves/" SHELF_ID "/atmos", data);
  });
  
}
