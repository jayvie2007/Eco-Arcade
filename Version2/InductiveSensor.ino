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
Servo Servo1;
int servoPin2 = D5; 
Servo Servo2;

//Setting up Inductive Proximity Sensor
const int inductiveSensorPin = D2;


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

    pinMode(inductiveSensorPin, INPUT);
    Servo1.attach(servoPin1); 
    Servo1.write(0); 
    Servo2.attach(servoPin2); 
    Servo2.write(180); 

}

void loop() {
    String start_opener = Firebase.getString("Servo/CanBlock");
    int inductiveSensorValue = digitalRead(inductiveSensorPin);
    int get_can_count = Firebase.getInt("Printer/Line2");
    int can_count = Firebase.getInt("BottleCount/can");
    Serial.println(inductiveSensorValue);
    if (start_opener == "start"){
      Servo2.write(90); 
      delay(2000);
      Servo2.write(180); 
      Firebase.setString("Servo/CanBlock", "stop");
    }
    // Get Current Can Count From Firebase
    else if (inductiveSensorValue == HIGH) {
        Firebase.setInt("BottleCount/can", can_count + 1);
        Firebase.setInt("Printer/Line2", get_can_count + 1);
        Firebase.setString("Printer/start", "start");
        Firebase.setString("Servo/Can", "start");
        Firebase.setString("BinResponse/message", "Can Detected!");
        Serial.println("Can Detected");
        Servo1.write(180); 
        Servo2.write(90); 
        delay(2000);
        Servo1.write(0); 
        Servo2.write(180); 
        Firebase.setString("BinResponse/message", "");
        Firebase.setString("Servo/Can","stop");
        delay(1000);
    }else{
            Serial.println("No Can Detected");
        }
}