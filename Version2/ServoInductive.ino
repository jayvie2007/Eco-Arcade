#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h> 

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

   Servo1.attach(servoPin1); 
}
void loop(){ 
   bool servo_starter = Firebase.getBool("Servo/Can");

   if (servo_starter == true){
      Servo1.write(180); 
      delay(2000);
      Servo1.write(0); 
      Firebase.setBool("Servo/Can",false);
   }
}