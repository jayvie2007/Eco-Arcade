#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h> 

//Setting up firebase and wifi connection
#define FIREBASE_HOST "eco-arcade-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XdKualT5QNGRWRWywe1d1lhNc9AL82ivhoge59v7"
// #define WIFI_SSID "OPPO A92"
// #define WIFI_PASSWORD "11111111"

#define WIFI_SSID "Smartbro-9EC6"
#define WIFI_PASSWORD "smartbro"

// Create a servo object 
int servoPin1 = D1; 
int servoPin2 = D2; 
Servo Servo1;
Servo Servo2;

//Setting up Capacitive Proximity for Paper Sensor
const int capacitiveSensorPinPlastic = D5;

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
    Servo2.attach(servoPin2); 
    Servo1.write(0); 
    Servo2.write(0); 

}

void loop() {
    // Get Readings From Capacitive Sensor Paper
    int capacitiveSensorPlasticValue = digitalRead(capacitiveSensorPinPlastic);
    
    // Get Current Paper Count From Firebase
    int paper_count = Firebase.getInt("BottleCount/paper");
    int plastic_count = Firebase.getInt("BottleCount/plastic");
    int get_plastic_count = Firebase.getInt("Printer/Line3");
    int get_paper_count = Firebase.getInt("Printer/Line4");
    String paper_starter = Firebase.getString("Servo/Paper");

    if (paper_starter == "start"){
        Firebase.setInt("BottleCount/paper", paper_count + 1);
        Firebase.setInt("Printer/Line4", get_paper_count + 1);
        Firebase.setString("Printer/start", "start");
        Firebase.setString("BinResponse/message", "Paper Detected!");
        Serial.println("Paper Detected");
        Servo2.write(180); 
        delay(2000);
        Servo2.write(0); 
        Firebase.setString("BinResponse/message", "");
        Firebase.setString("Servo/Paper","stop");
    } 
 
    Serial.println(capacitiveSensorPlasticValue);
    if (capacitiveSensorPlasticValue == HIGH) {
        Firebase.setInt("BottleCount/plastic", plastic_count + 1);
        Firebase.setInt("Printer/Line3", get_plastic_count + 1);
        Firebase.setString("Printer/start", "start");
        Firebase.setString("Servo/Plastic", "start");
        Firebase.setString("BinResponse/message", "Plastic Detected!");
        Serial.println("Plastic Detected");
        Servo1.write(180); 
        Servo2.write(180); 
        delay(2000);
        Servo1.write(0); 
        Servo2.write(0); 
        Firebase.setString("BinResponse/message", "");
        Firebase.setString("Servo/Plastic","stop");

    } else {
        Serial.println("No Plastic/Paper Detected");
    }
}