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

//Setting up Inductive Proximity Sensor
const int inductiveSensorPin = D1;

//Setting up Capacitive Proximity for Paper Sensor
const int capacitiveSensorPinPlastic = D2;

//Setting up Infared for Paper Sensor
// const int InfaredStarter = D3;

//Setting up Ultrasonic for Starter Sensor
const int echoPinStarter = D3; 
const int trigPinStarter = D4;
long durationStarter;
int distanceStarter;

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

    pinMode(trigPinStarter, OUTPUT);
    pinMode(echoPinStarter, INPUT);
    pinMode(inductiveSensorPin, INPUT);
    pinMode(capacitiveSensorPinPlastic, INPUT);
    // pinMode(InfaredStarter, INPUT);

}

void loop() {
    // Monitoring Sensors for Starter
    digitalWrite(trigPinStarter, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinStarter, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinStarter, LOW);
    durationStarter = pulseIn(echoPinStarter, HIGH);
    distanceStarter = durationStarter * 0.034 / 2;
    Serial.print("Distance Starter: ");
    Serial.println(distanceStarter);
    delay(500)

    // Get Readings From Infared Sensor Paper
    int InfaredStarterValue = digitalRead(InfaredStarter);

    // Trigger Starter
    // if(InfaredStarterValue == LOW) {
    //     // Set all value to false into Firebase
    //     Firebase.setBool("LinearTwo/starter", false);
    //     Firebase.setBool("LinearOne/starter", false);
    //     Firebase.setBool("Stepper/can", false);
    //     Firebase.setBool("Stepper/plastic", false);
    //     Firebase.setBool("Stepper/paper", false);
    //     Firebase.setBool("Stepper/start", true);
    // }

    if (distanceStarter <= 5){
        // Set all value to false into Firebase
        Firebase.setBool("LinearTwo/starter", false);
        Firebase.setBool("LinearOne/starter", false);
        Firebase.setBool("Stepper/can", false);
        Firebase.setBool("Stepper/plastic", false);
        Firebase.setBool("Stepper/paper", false);
        Firebase.setBool("Stepper/start", true);
    }
    
    //  Get Readings from Inductive Sensor
    int inductiveSensorValue = digitalRead(inductiveSensorPin);

    // Get Current Can Count From Firebase
    int can_count = Firebase.getInt("BottleCount/can");
    if (inductiveSensorValue == HIGH) {
        Firebase.setInt("BottleCount/can", can_count + 1);
        Firebase.setBool("LinearOne/starter", true);
        Firebase.setBool("Stepper/can", true);
        Serial.println("Can Detected");
        delay(10000);

    } else {
        Serial.println("No Can Detected");
    }

    // Get Readings From Capacitive Sensor Paper
    int capacitiveSensorPlasticValue = digitalRead(capacitiveSensorPinPlastic);

    // Get Current Paper Count From Firebase
    int plastic_count = Firebase.getInt("BottleCount/plastic");
    if (capacitiveSensorPlasticValue == HIGH) {
        Firebase.setInt("BottleCount/plastic", plastic_count + 1);
        Firebase.setBool("LinearTwo/starter", true);
        Firebase.setBool("Stepper/plastic", true);
        Serial.println("Plastic Detected");
        delay(10000);
        
    } else {
        Serial.println("No Plastic Detected");
    }
    delay(1000);
}