import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522
from datetime import datetime
class RFIDSensor():
    def __init__(self, multispi, spi_index, no_tag_timeout=5):
        self.mspi = multispi
        self.spi_index = spi_index
        self.mspi.select(self.spi_index)
        self.sensor = SimpleMFRC522()
        self.mspi.free()
        self.last_okay_read = None
        self.last_id_read = None
        self.id_lost_time = None
        self.no_tag_timeout = no_tag_timeout
        
    def get_state(self):
        self.mspi.select(self.spi_index)
        id = self.sensor.read_id_no_block()
        self.mspi.free()
        if(id is None):
            if(self.id_lost_time and (datetime.now()-self.id_lost_time).total_seconds()>self.no_tag_timeout and self.last_id_read is not None):
                self.last_id_read = None
                return (None, True)
            elif(not self.id_lost_time and self.last_id_read is not None):
                self.id_lost_time = datetime.now()
                return (self.last_okay_read, False)
        elif(id != self.last_id_read):
            self.last_okay_read = id
            self.last_id_read = id
            self.id_lost_time = None
            return (self.last_okay_read, True)
        else:
            self.id_lost_time = None
            self.last_id_read = id
        return (self.last_okay_read, False)
        
        
        
