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

#define TX_PIN 6 // Arduino TX pin -- RX on printer
#define RX_PIN 5 // Arduino RX pin -- TX on printer

SoftwareSerial printer_connection(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&printer_connection);

void setup()
{  
  printer_connection.begin(9600);
  printer.begin();

  printer.setFont('B');
  printer.println("Hello, World!");
  printer.setFont('A');
  printer.println("Hello, World!");

  printer.inverseOn();
  printer.println(F("Good Bye, World!"));
  printer.inverseOff();

  printer.doubleHeightOn();
  printer.println(F("Large Text"));
  printer.doubleHeightOff();
}

void loop() { }
