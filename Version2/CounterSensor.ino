#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//Setting up firebase and wifi connection
#define FIREBASE_HOST "eco-arcade-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XdKualT5QNGRWRWywe1d1lhNc9AL82ivhoge59v7"
// #define WIFI_SSID "OPPO A92"
// #define WIFI_PASSWORD "11111111"

#define WIFI_SSID "Smartbro-9EC6"
#define WIFI_PASSWORD "smartbro"

//Setting up PlasticCan Ultrasonic Sensor
const int echoPinCan = D5; //jumper D3
const int trigPinCan = D6; //jumper D4
long durationCan;
int distanceCan;
int binLevelCan;

//Setting up Capacitive Proximity for Paper Sensor
const int capacitiveSensorPinPaper = D1;

void setup() {
    Serial.begin(115200);
    // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    pinMode(trigPinCan, OUTPUT);
    pinMode(echoPinCan, INPUT);
    pinMode(capacitiveSensorPinPaper, INPUT);

}

void loop() {
    // Get Readings From Capacitive Sensor Paper
    int capacitiveSensorPaperValue = digitalRead(capacitiveSensorPinPaper);
    if (capacitiveSensorPaperValue == HIGH) {
      delay(3000);
      String can_starter = Firebase.getString("Servo/Can");
      String plastic_starter = Firebase.getString("Servo/Plastic");
      if (plastic_starter == "start" || can_starter == "start"){
        return;
      }
      else{
        Firebase.setString("Servo/Paper", "start");
        Firebase.setString("Servo/CanBlock", "start");
        delay(2000);
      } 
    }
  
    // Bin Level Monitoring Sensors for Can
    digitalWrite(trigPinCan, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinCan, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinCan, LOW);
    durationCan = pulseIn(echoPinCan, HIGH);
    distanceCan = durationCan * 0.034 / 2;
    Serial.print("Distance Can: ");
    Serial.println(distanceCan);
    //BinCan
    if (distanceCan <= 3) {
        binLevelCan = 10;
    }
    else if (distanceCan <=6 ) {
        binLevelCan = 9;
        }
    else if (distanceCan <=9 ) {
        binLevelCan = 8;
        }
    else if (distanceCan <=12 ) {
        binLevelCan = 7;
        }
    else if (distanceCan <=15 ) {
        binLevelCan = 6;
        }
    else if (distanceCan <=18 ) {
        binLevelCan = 5;
        }
    else if (distanceCan <=21 ) {
        binLevelCan = 4;
        }
    else if (distanceCan <=24 ) {
        binLevelCan = 3;
        }
    else if (distanceCan <=27 ) {
        binLevelCan = 2;
        }
    else if (distanceCan >= 30) {
        binLevelCan = 1;
    }
      Firebase.setFloat("Bin/can", binLevelCan);
}