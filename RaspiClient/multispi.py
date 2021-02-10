import RPi.GPIO as GPIO
class MultiSPIHandler():
    def __init__(self, pinmap):
        self.pinmap = pinmap
        self.current = -1
        for pin in self.pinmap:
            GPIO.setup(pin, GPIO.OUT)
            GPIO.output(pin, GPIO.HIGH)
    def select(self, device):
        if(device<len(self.pinmap)):
            GPIO.output(self.pinmap[device], GPIO.LOW)
            self.current = device
    def free(self):
        if(self.current != -1):
            GPIO.output(self.pinmap[self.current], GPIO.HIGH)
            self.current = -1