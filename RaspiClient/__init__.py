import sys
import RPi.GPIO as GPIO
from weight_sensor import WeightSensor
from rfid_sensor import RFIDSensor
import paho.mqtt.client as mqtt
from config import *
import json
from multispi import MultiSPIHandler
def exitApp():
    GPIO.cleanup()
    sys.exit()
print("Starting")
GPIO.setmode(GPIO.BCM)
multispi = MultiSPIHandler([
    13,
    19,
    26
])
weight_sensors = [
    WeightSensor(1325,5,6),
]
print("Created weight sensors")
rfid_sensors = [
    RFIDSensor(multispi, 0),
    RFIDSensor(multispi, 1),
    RFIDSensor(multispi, 2),
]
print("Created RFID sensors")
atmospheric_sensor = [

]
print("Created atmos sensors")
def on_connect(client, userdata, flags, rc):
    print("Connected to mqtt broker")
    client.subscribe(MQTT_TOPIC_PRIMARY+"/"+MQTT_TOPIC_SECONDARY+"/"+MQTT_TOPIC_COMMAND)

def on_message(client, userdata, msg):
    print("""{{
        \t{0}
        \t{1}
        }}""".format(msg.topic, msg.payload))
    print(msg.topic)
    print(msg.payload)
    if(msg.payload == b"getall"):
        data = []
        for i in range(len(weight_sensors)):
            weight, has_weight_changed = weight_sensors[i].get_state()
            rfid_tag, has_rfid_changed = rfid_sensors[i].get_state()
            changed_data = {
                "position" : i,
                "rfid_tag" : rfid_tag,
                "weight"   : weight
            }
            data.append(changed_data)
        print("Returning all sensor values")
        mqtt_client.publish(
            MQTT_TOPIC_PRIMARY+"/"+MQTT_TOPIC_SECONDARY+"/"+MQTT_TOPIC_SENSOR_DATA,
            json.dumps(data)
        )
    

mqtt_client = mqtt.Client(MQTT_NAME)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
mqtt_client.connect("192.168.1.133")
while True:
    try:
        for sensor in rfid_sensors:
            print(sensor.get_state())
        print("-------------------")
    except (KeyboardInterrupt, SystemExit):
        exitApp()

while True:
    try:
        mqtt_client.loop()
        data = []
        for i in range(len(weight_sensors)):
            weight, has_weight_changed = weight_sensors[i].get_state()
            rfid_tag, has_rfid_changed = rfid_sensors[i].get_state()
            changed_data = {
                "position" : i,
            }
            if(has_rfid_changed):
                changed_data["rfid_tag"] = rfid_tag
            if(has_weight_changed):
                changed_data["weight"] = weight
            if(len(changed_data)>1):
                data.append(changed_data)
        if(len(data)>0):
            print("Publishing")
            mqtt_client.publish(
                MQTT_TOPIC_PRIMARY+"/"+MQTT_TOPIC_SECONDARY+"/"+MQTT_TOPIC_SENSOR_DATA,
                json.dumps(data)
            )
    except (KeyboardInterrupt, SystemExit):
        exitApp()


