linear.py
from datetime import datetime
from firebase_settings import db

import os
import sys
import requests

import time
import board
import RPi.GPIO as GPIO

url = "https://www.google.com"
timeout = 5

def restart():
    os.execv(sys.executable,['python3'] + sys.argv)
    
LinearOpen = 19 #PIN=35 LEFT
LinearClose = 26 #PIN=37 LEFT

GPIO.setup(LinearOpen, GPIO.OUT)
GPIO.setup(LinearClose, GPIO.OUT)
GPIO.output(LinearOpen, GPIO.OUT)
GPIO.output(LinearClose, GPIO.OUT)

def open_motor(delay):
    GPIO.output(LinearOpen, GPIO.HIGH)
    GPIO.output(LinearClose, GPIO.LOW)
    time.sleep(delay)
    
def close_motor(delay):
    GPIO.output(LinearOpen, GPIO.LOW)
    GPIO.output(LinearClose, GPIO.HIGH)
    time.sleep(delay)

while True:
    try:
        check_starter = db().child("LinearOne").get().val()
        existing_starter = check_starter["starter"]
    except:
        db().child("LinearOne").update({"starter":False})
        check_starter = db().child("LinearOne").get().val()
        existing_starter = check_starter["starter"]    

    try:
        if existing_starter:
            open_motor(10)
            db().child("LinearOne").update({"starter":False})
            db().child("Stepper").update({"plastic":False})
            close_motor(15)
    except:
        print("connecting")
        restart() 

