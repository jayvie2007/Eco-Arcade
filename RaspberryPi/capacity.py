from time import sleep
from datetime import datetime
from gpiozero import InputDevice

sensor_pin = 4 #GPIO
sensor = InputDevice(sensor_pin)
count = 0

try:
    while True:
        # Read and print the raw state of the sensor
        print("Sensor State:", sensor.value)
        sleep(0.2)

except KeyboardInterrupt:
    print("Cleaning up!")