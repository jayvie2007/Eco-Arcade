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

//Setting up PlasticBin Ultrasonic Sensor
const int echoPinPlastic = 5; //jumper D1
const int trigPinPlastic = 4; //jumper D2
long durationPlastic;
int distancePlastic;
int binLevelPlastic;

//Setting up PlasticCan Ultrasonic Sensor
const int echoPinCan = 0; //jumper D3
const int trigPinCan = 2; //jumper D4
long durationCan;
int distanceCan;
int binLevelCan;

//Setting up PaperBin Ultrasonic Sensor
const int echoPinPaper = 14; //jumper D5
const int trigPinPaper = 12; //jumper D6
long durationPaper;
int distancePaper;
int binLevelPaper;

//Setting up Infared Sensor for Paper Sensor
const int infaredSensorPinPaper = D7;

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
    pinMode(trigPinCan, OUTPUT);
    pinMode(echoPinCan, INPUT);
    pinMode(trigPinPaper, OUTPUT);
    pinMode(echoPinPaper, INPUT);
    pinMode(infaredSensorPinPaper, INPUT);
}

void loop() {
    // Get Readings From Infared Sensor Paper
    int infaredSensorPaperValue = digitalRead(infaredSensorPinPaper);

    // Get Current Paper Count From Firebase
    int paper_count = Firebase.getInt("BottleCount/paper"); 
    if(infaredSensorPaperValue == LOW) {
        Firebase.setInt("BottleCount/paper", paper_count + 1);
        Firebase.setInt("Stepper/paper", true);
        Firebase.setBool("Stepper/stop", false);
        Serial.println("Paper Detected");
        delay(5000);
        
    }
    else{
        Serial.println("No Paper Detected");
    }

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
    if (distancePlastic <= 8) {
        binLevelPlastic = 3;
    }
    else if (distancePlastic <=14 ) {
        binLevelPlastic = 2;
        }
    else if (distancePlastic >= 18) {
        binLevelPlastic = 1;
    }

    //BinCan
    if (distanceCan <= 8) {
        binLevelCan = 3;
    }
    else if (distanceCan <=14 ) {
        binLevelCan = 2;
    }
    else if (distanceCan >= 18) {
        binLevelCan = 1;
    }
        
    //BinPaper
    if (distancePaper <= 8) {
        binLevelPaper = 3;
    }
    else if (distancePaper <=14 ) {
        binLevelPaper = 2;
    }
    else if (distancePaper >= 18) {
        binLevelPaper = 1;
    }

    //Pass Value to Firebase
    Firebase.setFloat("Bin/plastic", binLevelPlastic);
    Firebase.setFloat("Bin/can", binLevelCan);
    Firebase.setFloat("Bin/paper", binLevelPaper);

    if (Firebase.failed()) {
        Serial.print("setting /number failed:");
        Serial.println(Firebase.error());  
        return;
    }
    delay(500);
}