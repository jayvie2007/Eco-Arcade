from firebase_settings import db

import RPi.GPIO as GPIO
import board
import time

#STEPPER
out1 = 17 #PIN=11 LEFT
out2 = 18 #PIN=12 RIGHT
out3 = 27 #PIN=13 LEFT
out4 = 22 #PIN=15 LEFT
step_sleep = 0.002
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

def stepper_rotate_right(step_count):
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

while True:    
    try:
        check_starter = db().child("Stepper").get().val()
        existing_starter = check_starter["starter"]
        existing_can = check_starter["can"]
        existing_plastic = check_starter["plastic"]
        existing_paper = check_starter["paper"]
    except:
        db().child("Stepper").update({"starter":False,"can":False,"plastic":False,"paper":False})
        check_starter = db().child("Stepper").get().val()
        existing_starter = check_starter["starter"]

    # Initial Trigger        
    if existing_starter:
        stepper_rotate_right(200)
        db().child("Stepper").update({"starter":False})
        time.sleep(5)
        check_starter = db().child("Stepper").get().val()
        existing_can = check_starter["can"]
    
        # If can not detected move motor 
        if not existing_can:
            stepper_rotate_right(200)
            time.sleep(5)
            check_starter = db().child("Stepper").get().val()
            existing_can = check_starter["can"]

            # If can plastic detected move motor 
            if not existing_plastic:
                stepper_rotate_right(200)
                time.sleep(5)
                check_starter = db().child("Stepper").get().val()
                existing_plastic = check_starter["plastic"]

                # If can plastic detected move motor 
                if not existing_paper:
                    stepper_rotate_right(600)
        

