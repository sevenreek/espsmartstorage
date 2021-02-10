#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "NodeMCUPins.h"

#define RFID_BYTE_SIZE 4 // number of bytes in the rfid tags

#define TARE_ON_START 1 // if enabled will tare on start, useful for debugging
#define SENSORS_COUNT 2 // number of product spots
#define SHELF_ID "1"  // shelf id for mqtt topics
#define MQTT_POST_DATA_TOPIC "shelves/" SHELF_ID "/data"  // mqtt topic to send sensor data to
#define SHELF_POSITION_0 "0"  // the positions of the spots on the shelf used in mqtt topics
#define SHELF_POSITION_1 "1"  // the positions of the spots on the shelf used in mqtt topics

#define HX711_DATA_PIN_0 D1 // data pin for the first hx711
#define HX711_DATA_PIN_1 D0 // data pin for the second hx711
#define HX711_SCALE_0 -1387  // scale divider for the first hx711
#define HX711_SCALE_1 1300  // scale divider for the second hx711
#define HX711_CLOCK_PIN D2  // clock pin of the hx711
#define WEIGHT_PRECISION 10 // difference in the weight required to send a mqtt message

#define CALIBRATION_WEIGHT 500 // mass of the calibration weight to be used for calibration

#define RC522_RST_PIN_0         UINT8_MAX    // reset pin of the first mfrc522; set to UINT8_MAX to disable
#define RC522_SS_PIN_0          D4           // ss pin of the first mfrc522

#define RC522_RST_PIN_1         UINT8_MAX      // reset pin of the second mfrc522; set to UINT8_MAX to disable
#define RC522_SS_PIN_1          D3             // ss pin of the second mfrc522


#define TARE_MA_REPEAT 8          // number of times to repeat measurement for taring

//#define ENABLE_SERIAL // uncomment to enable serial communication, disables LEDs

#define CMD_GETALL "getall" // mqtt topic to return all sensor values
#define CMD_TARE "tare" // mqtt topic to tare the weight sensors
#define CMD_CALIBRATE "calibrate" // mqtt topic to calibrate the weight sensors
#define CMD_LIGHT "light" // mqtt topic to toggle the LEDs
#endif
