
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

//Setting up Capacitive Proximity for Paper Sensor
const int capacitiveSensorPinPlastic = D1;

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

}

void loop() {
    // Get Readings From Capacitive Sensor Paper
    int capacitiveSensorPlasticValue = digitalRead(capacitiveSensorPinPlastic);

    // Get Current Paper Count From Firebase
    int plastic_count = Firebase.getInt("BottleCount/plastic");
    if (capacitiveSensorPlasticValue == HIGH) {
        Firebase.setInt("BottleCount/plastic", plastic_count + 1);
        Firebase.setBool("Printer/start", true);.
        Firebase.setBool("ServoPlastic/start", true);
        Firebase.setString("BinResponse/message", "Can Detected!");
        Serial.println("Plastic Detected");
        delay(10000);
        Firebase.setBool("ServoPlastic/start", false);
        Firebase.setString("BinResponse/message", "");

    } else {
        Serial.println("No Plastic Detected");
    }
    delay(1000);
}