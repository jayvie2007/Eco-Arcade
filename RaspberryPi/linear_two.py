from datetime import datetime
from firebase_settings import db

import os
import sys
import time
import board
import RPi.GPIO as GPIO

LinearOpen = 16 #PIN=36 RIGHT
LinearClose = 20 #PIN=38 RIGHT

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
        check_starter = db().child("LinearTwo").get().val()
        existing_starter = check_starter["starter"]
    except:
        db().child("LinearTwo").update({"starter":False})
        check_starter = db().child("LinearTwo").get().val()
        existing_starter = check_starter["starter"]    

    if existing_starter:
        open_motor(5000)
        db().child("LinearTwo").update({"starter":False})
        close_motor(5000)

