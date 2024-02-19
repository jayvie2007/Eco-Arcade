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

    // Get Current Paper Count From Firebase
    int paper_count = Firebase.getInt("BottleCount/paper");
    if (capacitiveSensorPaperValue == HIGH) {
        Firebase.setInt("BottleCount/paper", paper_count + 1);
        Firebase.setBool("Printer/start", true);
        Firebase.setBool("ServoPaper/start", true);
        Firebase.setString("BinResponse/message", "Can Detected!");
        Serial.println("Paper Detected");
        delay(10000);
        Firebase.setBool("ServoPaper/start", false);
        Firebase.setString("BinResponse/message", "");
        
    } else {
        Serial.println("No Paper Detected");
    }
    delay(1000);
}