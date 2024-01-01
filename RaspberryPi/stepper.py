from firebase_settings import db

import RPi.GPIO as GPIO
import board
import time

#STEPPER
out1 = 17
out2 = 18
#out2 = 12
out3 = 27
out4 = 22
step_sleep = 0.002
step_count = 1000
initialize_count = 0

# setting up
GPIO.setup( out1, GPIO.OUT )
GPIO.setup( out2, GPIO.OUT )
GPIO.setup( out3, GPIO.OUT )
GPIO.setup( out4, GPIO.OUT )
# initializing
GPIO.output( out1, GPIO.LOW )
GPIO.output( out2, GPIO.LOW )
GPIO.output( out3, GPIO.LOW )
GPIO.output( out4, GPIO.LOW )

def cleanup():
    GPIO.output( out1, GPIO.LOW )
    GPIO.output( out2, GPIO.LOW )
    GPIO.output( out3, GPIO.LOW )
    GPIO.output( out4, GPIO.LOW )

def stepper_rotate_right():
    for initialize_count in range(step_count):
        if initialize_count%4==0:
            GPIO.output( out4, GPIO.LOW )
            GPIO.output( out3, GPIO.LOW )
            GPIO.output( out2, GPIO.LOW )
            GPIO.output( out1, GPIO.HIGH )
        elif initialize_count%4==1:
            GPIO.output( out4, GPIO.LOW )
            GPIO.output( out3, GPIO.HIGH )
            GPIO.output( out2, GPIO.LOW )
            GPIO.output( out1, GPIO.LOW )
        elif initialize_count%4==2:
            GPIO.output( out4, GPIO.LOW )
            GPIO.output( out3, GPIO.LOW )
            GPIO.output( out2, GPIO.HIGH )
            GPIO.output( out1, GPIO.LOW )
        elif initialize_count%4==3:
            GPIO.output( out4, GPIO.HIGH )
            GPIO.output( out3, GPIO.LOW )
            GPIO.output( out2, GPIO.LOW )
            GPIO.output( out1, GPIO.LOW )
        time.sleep(step_sleep)
        if initialize_count == step_count - 1:
            cleanup()

def stepper_rotate_left():
    for initialize_count in range(step_count):
        if initialize_count%4==0:
            GPIO.output( out4, GPIO.HIGH )
            GPIO.output( out3, GPIO.LOW )
            GPIO.output( out2, GPIO.LOW )
            GPIO.output( out1, GPIO.LOW )
        elif initialize_count%4==1:
            GPIO.output( out4, GPIO.LOW )
            GPIO.output( out3, GPIO.LOW )
            GPIO.output( out2, GPIO.HIGH )
            GPIO.output( out1, GPIO.LOW )
        elif initialize_count%4==2:
            GPIO.output( out4, GPIO.LOW )
            GPIO.output( out3, GPIO.HIGH )
            GPIO.output( out2, GPIO.LOW )
            GPIO.output( out1, GPIO.LOW )
        elif initialize_count%4==3:
            GPIO.output( out4, GPIO.LOW )
            GPIO.output( out3, GPIO.LOW )
            GPIO.output( out2, GPIO.LOW )
            GPIO.output( out1, GPIO.HIGH )
        time.sleep(step_sleep)        
        if initialize_count == step_count - 1:
            cleanup()

try:
    check_starter = db().child("Stepper").get()
    existing_starter = check_starter["starter"]
except:
    db().child("Stepper").update({"starter":False})
    check_starter = db().child("Stepper").get()
    existing_starter = check_starter["starter"]

while True:    
    if existing_starter:
        stepper_rotate_right()
        db().child("Stepper").update({"starter":False})

