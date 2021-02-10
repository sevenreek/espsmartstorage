
#include "WeightSensorsController.h"
#include "RFIDControllers.h"
#include "LEDController.h"
#include <EspMQTTClient.h>





EspMQTTClient client(
  SSID,
  PASSWORD,
  MQTT_IP,  // MQTT Broker server ip
  "ESP8266_SP01",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

byte HX711_DATA_PINS[SENSORS_COUNT] = {HX711_DATA_PIN_0, HX711_DATA_PIN_1};
int HX711_SCALE[SENSORS_COUNT] = {HX711_SCALE_0, HX711_SCALE_1};
byte RC522_SS_PINS[SENSORS_COUNT] = {RC522_SS_PIN_0, RC522_SS_PIN_1};
byte RC522_RST_PINS[SENSORS_COUNT] = {RC522_RST_PIN_0, RC522_RST_PIN_1};

byte LED_PINS[SENSORS_COUNT] = {RX, TX};
char* SHELF_POSITIONS[SENSORS_COUNT] = {SHELF_POSITION_0, SHELF_POSITION_1};
WeightSensorsController weightSensors(HX711_DATA_PINS, HX711_CLOCK_PIN, HX711_SCALE, WEIGHT_PRECISION);
RFIDSensorsController rfidController(RC522_SS_PINS, RC522_RST_PINS);
LEDController ledController(LED_PINS);
//AtmosSensorController atmosController(TX, D8, 0x76, 0.1f, 1.0f, 100.0f);
void setup() {
#ifdef ENABLE_SERIAL
	Serial.begin(9600);		// Initialize serial communications with the PC
  while(!Serial);
  Serial.println(F("Starting"));
#else
  ledController.begin();
#endif
  rfidController.begin();
  //atmosController.begin();
#if TARE_ON_START
  for(int i=0; i< SENSORS_COUNT; i++)
  {
    weightSensors.tare(i);
  }
#endif
  
 
#ifdef ENABLE_SERIAL
	Serial.println(F("Running"));
 //client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
 #endif
}

void loop() {


  client.loop();
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  uint8_t weightsChanged = weightSensors.isChanged();
  uint8_t rfidChanged = rfidController.isChanged();
  /*if(atmosController.isChanged())
  {
    Serial.print("T:");Serial.print(atmosController.lastT);Serial.print("; ");
    Serial.print("H:");Serial.print(atmosController.lastH);Serial.print("; ");
    Serial.print("P:");Serial.print(atmosController.lastP);Serial.println("");
  }*/
  String data = "[";
  bool appendedAChangeToString = false;
  for(int i=0; i< SENSORS_COUNT; i++)
  {
    if( appendedAChangeToString && i>0 && ((rfidChanged & (0b1<<i)) || (weightsChanged & (0b1<<i))) )
      data += ",";
    if( (rfidChanged & (0b1<<i)) && (weightsChanged & (0b1<<i)))
    {
      data += "{\"pos\":" + String(SHELF_POSITIONS[i]) + ",\"id\":\"";
      byte* buffer;
      uint8_t size = rfidController.getLastID(i,&buffer);
      for (byte i = 0; i < size; i++) {
        data += (buffer[i] < 0x10 ? "0" : "");
        data += String(buffer[i], HEX);
      }
      data += "\",\"w\":"+String(weightSensors.results[i])+"}";
      appendedAChangeToString = true;
    }
    else if(rfidChanged & (0b1<<i))
    {
      data += "{\"pos\":" + String(SHELF_POSITIONS[i]) + ",\"id\":\"";
      byte* buffer;
      uint8_t size = rfidController.getLastID(i,&buffer);
      for (byte i = 0; i < size; i++) {
        data += (buffer[i] < 0x10 ? "0" : "");
        data += String(buffer[i], HEX);
      }
      data += "\"}";
      appendedAChangeToString = true;
    }
    else if(weightsChanged & (0b1<<i))
    {
      data += "{\"pos\":" + String(SHELF_POSITIONS[i]) + ",";
      data += "\"w\":" + String(weightSensors.results[i]) +"}";
      appendedAChangeToString = true;
    }
  }
  
  data += "]";
  if(weightsChanged || rfidChanged)
  {
    client.publish("shelves/1/data", data); // You can activate the retain flag by setting the third parameter to true
  }
  
}

/*
// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, uint8_t bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}*/
/*
void tare() {
  bool tareSuccessful = false;

  unsigned long tareStartTime = millis();
  while (!tareSuccessful && millis()<(tareStartTime+TARE_TIMEOUT_SECONDS*1000)) {
    tareSuccessful = WeightSensors.tare(20,10000);  //reject 'tare' if still ringing
  }
}*/

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("shelves/" SHELF_ID "/" CMD_GETALL, [](const String & payload) {
    String data = "[";
    for(int i=0; i< SENSORS_COUNT; i++)
    {

        data += "{\"pos\":" + String(SHELF_POSITIONS[i]) + ",\"id\":\"";
        byte* buffer;
        uint8_t size = rfidController.getLastID(i,&buffer);
        for (byte i = 0; i < size; i++) {
          data += (buffer[i] < 0x10 ? "0" : "");
          data += String(buffer[i], HEX);
        }
        data += "\",\"w\":"+String(weightSensors.results[i])+"}";
        if(i<SENSORS_COUNT-1) data += ",";
    }
    data += "]";
    client.publish("shelves/" SHELF_ID "/data", data);
  });
  client.subscribe("shelves/" SHELF_ID "/" CMD_TARE "/" SHELF_POSITION_0, [](const String & payload) {
      weightSensors.tare(0);
  });
  client.subscribe("shelves/" SHELF_ID "/" CMD_CALIBRATE "/" SHELF_POSITION_0, [](const String & payload) {
      weightSensors.calibrate(0, CALIBRATION_WEIGHT);
  });

  client.subscribe("shelves/" SHELF_ID "/" CMD_TARE "/" SHELF_POSITION_1, [](const String & payload) {
      weightSensors.tare(1);
  });
  client.subscribe("shelves/" SHELF_ID "/" CMD_CALIBRATE "/" SHELF_POSITION_1, [](const String & payload) {
      weightSensors.calibrate(1, CALIBRATION_WEIGHT);
  });

  client.subscribe("shelves/" SHELF_ID "/" CMD_LIGHT "/" SHELF_POSITION_0, [](const String & payload) {
    #ifndef ENABLE_SERIAL
      ledController.toggle(0);
    #else
      Serial.println("Toggling led 0");
    #endif
  });
  client.subscribe("shelves/" SHELF_ID "/" CMD_LIGHT "/" SHELF_POSITION_1, [](const String & payload) {
    #ifndef ENABLE_SERIAL
      ledController.toggle(1);
    #else
      Serial.println("Toggling led 1");
    #endif
  });

  
}
