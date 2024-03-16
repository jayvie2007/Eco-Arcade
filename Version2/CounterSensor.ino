#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//Setting up firebase and wifi connection
#define FIREBASE_HOST "eco-arcade-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XdKualT5QNGRWRWywe1d1lhNc9AL82ivhoge59v7"
#define WIFI_SSID "OPPO A92"
#define WIFI_PASSWORD "11111111"

// #define WIFI_SSID "ECO WIFI"
// #define WIFI_PASSWORD "11111111"

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
        delay(2000);
      } 
    delay(500);
  }
}