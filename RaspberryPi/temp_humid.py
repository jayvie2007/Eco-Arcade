from firebase_settings import db
from time import sleep, strftime
from luma.core.interface.serial import spi, noop
from luma.core.render import canvas
from luma.core.virtual import viewport
from luma.led_matrix.device import max7219
from luma.core.legacy import text, show_message
from luma.core.legacy.font import proportional, CP437_FONT, LCD_FONT,TINY_FONT

import RPi.GPIO as GPIO
import adafruit_dht
import board
import time

#PIN=40 RIGHT
dht = adafruit_dht.DHT22(board.D21) 
dht = adafruit_dht.DHT22(board.D21, use_pulseio=False)

#PIN DIN=19 LEFT CLK=23 LEFT CS=24 RIGHT
serial = spi(port=0, device=0, gpio=noop())
device = max7219(serial, width=32, height=8, block_orientation=-90, blocks_arranged_in_reverse_order=False)
device.contrast(100)
virtual = viewport(device, width=32, height=16)

#PIN DIN=19 LEFT CLK=23 LEFT CS=26 RIGHT
serial_two = spi(port=0, device=1, gpio=noop())
device_two = max7219(serial_two, width=32, height=8, block_orientation=-90,blocks_arranged_in_reverse_order=False)
virtual_two = viewport(device_two, width=32, height=16)
device_two.contrast(100)

while True:
    try:
        temperature = dht.temperature
        temp = ("T:{:.1f}".format(temperature))
        humidity = dht.humidity
        humid = f"H:{humidity}"
        check_starter = db().child("Stepper").get().val()
        existing_can = check_starter["can"]
        existing_plastic = check_starter["plastic"]
        existing_paper = check_starter["paper"]
        existing_stop = check_starter["stop"]
        #Check if humid and temp catches value else 0
        if not humid:
            humidty = 0
            humid = ("H:0")
        if not temp:
            temperature = 0
            temp = ("T:0")

        #First Count
        if existing_can: 
            result = "CN + 1"
        elif existing_plastic:
            result = "PL + 1"
        elif existing_paper:
            result = "PP + 1"
        elif existing_stop:
            result = "WAIT"
        else:
            result = ""

        with canvas(virtual) as draw:
            text(draw, (0, 1), result, fill="white", font=proportional(LCD_FONT))
        with canvas(virtual_two) as draw:
            text(draw, (0, 1), temp, fill="white", font=proportional(LCD_FONT))
        db().child("Weather").update({"temperature":temperature,"humidity":humidity})
        time.sleep(5)

        #Second Count
        with canvas(virtual) as draw:
            text(draw, (0, 1), result, fill="white", font=proportional(LCD_FONT))
        with canvas(virtual_two) as draw:
            text(draw, (0, 1), humid, fill="white", font=proportional(LCD_FONT))
        db().child("Weather").update({"temperature":temperature,"humidity":humidity})
        time.sleep(5)

    except RuntimeError as e:
        print("Reading from DHT failure: ", e.args)
        time.sleep(0.5)
    except KeyboardInterrupt:
        GPIO.cleanup()
        print("Stopped")