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

//Setting up PlasticBin Ultrasonic Sensor
const int echoPinPlastic = 5; //jumper D1
const int trigPinPlastic = 4; //jumper D2
long durationPlastic;
int distancePlastic;
int binLevelPlastic;

//Setting up PaperBin Ultrasonic Sensor
const int echoPinPaper = 14; //jumper D5
const int trigPinPaper = 12; //jumper D6
long durationPaper;
int distancePaper;
int binLevelPaper;


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

    pinMode(trigPinPlastic, OUTPUT);
    pinMode(echoPinPlastic, INPUT);
    pinMode(trigPinPaper, OUTPUT);
    pinMode(echoPinPaper, INPUT);
}

void loop() {
    // Bin Level Monitoring Sensors for Plastic
    digitalWrite(trigPinPlastic, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinPlastic, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinPlastic, LOW);
    durationPlastic = pulseIn(echoPinPlastic, HIGH);
    distancePlastic = durationPlastic * 0.034 / 2;
    Serial.print("Distance Plastic: ");
    Serial.println(distancePlastic);

    // Bin Level Monitoring Sensors for Paper
    digitalWrite(trigPinPaper, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinPaper, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinPaper, LOW);
    durationPaper = pulseIn(echoPinPaper, HIGH);
    distancePaper = durationPaper * 0.034 / 2;
    Serial.print("Distance Paper: ");
    Serial.println(distancePaper);

    // BinPlastic
    if (distancePlastic <= 3) {
        binLevelPlastic = 10;
    }
    else if (distancePlastic <=6 ) {
        binLevelPlastic = 9;
        }
    else if (distancePlastic <=9 ) {
        binLevelPlastic = 8;
        }
    else if (distancePlastic <=12 ) {
        binLevelPlastic = 7;
        }
    else if (distancePlastic <=15 ) {
        binLevelPlastic = 6;
        }
    else if (distancePlastic <=18 ) {
        binLevelPlastic = 5;
        }
    else if (distancePlastic <=21 ) {
        binLevelPlastic = 4;
        }
    else if (distancePlastic <=24 ) {
        binLevelPlastic = 3;
        }
    else if (distancePlastic <=27 ) {
        binLevelPlastic = 2;
        }
    else if (distancePlastic >= 30) {
        binLevelPlastic = 1;
    }
        
    //BinPaper
    if (distancePaper <= 3) {
        binLevelPaper = 10;
    }
    else if (distancePaper <=6 ) {
        binLevelPaper = 9;
        }
    else if (distancePaper <=9 ) {
        binLevelPaper = 8;
        }
    else if (distancePaper <=12 ) {
        binLevelPaper = 7;
        }
    else if (distancePaper <=15 ) {
        binLevelPaper = 6;
        }
    else if (distancePaper <=18 ) {
        binLevelPaper = 5;
        }
    else if (distancePaper <=21 ) {
        binLevelPaper = 4;
        }
    else if (distancePaper <=24 ) {
        binLevelPaper = 3;
        }
    else if (distancePaper <=27 ) {
        binLevelPaper = 2;
        }
    else if (distancePaper >= 30) {
        binLevelPaper = 1;
    }

    //Pass Value to Firebase
    Firebase.setFloat("Bin/plastic", binLevelPlastic);
    Firebase.setFloat("Bin/paper", binLevelPaper);

    if (Firebase.failed()) {
        Serial.print("setting /number failed:");
        Serial.println(Firebase.error());  
        return;
    }
    delay(500);
}