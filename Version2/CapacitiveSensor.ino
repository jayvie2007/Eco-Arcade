
@@ -0,0 +1,54 @@
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

// Create a servo object 
int servoPin1 = D1; 
Servo Servo1;

//Setting up Capacitive Proximity for Paper Sensor
const int capacitiveSensorPinPlastic = D2;

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

    pinMode(capacitiveSensorPinPlastic, INPUT);
    Servo1.attach(servoPin1); 


}

void loop() {
    // Get Readings From Capacitive Sensor Paper
    int capacitiveSensorPlasticValue = digitalRead(capacitiveSensorPinPlastic);

    // Get Current Paper Count From Firebase
    int plastic_count = Firebase.getInt("BottleCount/plastic");
    if (capacitiveSensorPlasticValue == HIGH) {
        Firebase.setInt("BottleCount/plastic", plastic_count + 1);
        Firebase.setBool("Printer/start", true);.
        Firebase.setBool("Servo/Plastic", true);
        Firebase.setString("BinResponse/message", "Can Detected!");
        Serial.println("Plastic Detected");
        Servo1.write(0); 
        delay(2000);
        Servo1.write(180); 
        Firebase.setString("BinResponse/message", "");
        Firebase.setString("Servo/Can","stop");

    } else {
        Serial.println("No Plastic Detected");
    }
    delay(1000);
}