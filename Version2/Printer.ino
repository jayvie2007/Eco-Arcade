#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//Setting up firebase and wifi connection
#define FIREBASE_HOST "eco-arcade-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XdKualT5QNGRWRWywe1d1lhNc9AL82ivhoge59v7"
#define WIFI_SSID "OPPO A92"
#define WIFI_PASSWORD "11111111"

int is_switch_press = 0; // For detecting the switch press status
int debounce_delay = 300; //Debounce delay
int switch_button = D1;

void setup()
{  
  pinMode(switch_button, INPUT);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  is_switch_press = digitalRead(switch_button); // Reading the Switch press status
  
  String line1 = Firebase.getString("Printer/Line1")
  String line2 = Firebase.getString("Printer/Line2")
  String line3 = Firebase.getString("Printer/Line3")
  String line4 = Firebase.getString("Printer/Line4")
  String line5 = Firebase.getString("Printer/Line5")
  String printer_starter = Firebase.getString("Printer/Start")

  if (is_switch_press == HIGH){
      delay(debounce_delay); // debounce delay for button press
      if (printer_starter == "start"){
        if (is_switch_press == HIGH){
          Serial.println(line1);
          Serial.println("---------------------------- \n\r");
          Serial.println("\n\r");
          Serial.println("\n\r");
          Serial.println("\n\r");
          delay(100);
          Serial.println(line2);
          Serial.println(line3);
          delay(100);
          Serial.println("\n\r");
          Serial.println("\n\r");
          Serial.println("---------------------------- \n\r");
          Serial.println(line4);
          Serial.println(line5);
          Serial.println("Thank You.");
          Serial.println("\n\r");
          Serial.println("\n\r");
          Serial.println("\n\r");
          Firebase.setString("Printer/Start","stop");
        }    
    }
    else{
      Firebase.setString("BinResponse/Message","Insert a bottle first");
      delay(1000)
      Firebase.setString("BinResponse/Message","");
    }
  }
}
