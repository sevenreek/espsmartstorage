import time
import sys
import RPi.GPIO as GPIO
from hx711 import HX711

class WeightSensor():
    def __init__(self, reference_unit:int, data_out_pin:int, sck_pin:int):
        self.reference_unit = reference_unit
        self.sensor = HX711(data_out_pin, sck_pin)
        self.sensor.set_reading_format("MSB", "MSB")
        self.sensor.set_reference_unit(self.reference_unit)
        self.sensor.reset()
        self.tare()
        self.last_reading = self.get_raw()
    def tare(self):
        self.sensor.tare()
    def get_state(self, sample_num:int=5, precision=1):
        value = self.get_raw(sample_num)
        if(abs(value-self.last_reading)>precision):
            self.last_reading = value
            return (value, True)
        else:
            return (self.last_reading, False)
    def get_raw(self, sample_num:int=5):
        return self.sensor.get_weight(sample_num)



