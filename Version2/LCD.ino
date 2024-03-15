#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define DHTPIN D3

float humidity;
float temperature;
DHT dht(DHTPIN, DHTTYPE);

//Setting up firebase and wifi connection
#define FIREBASE_HOST "eco-arcade-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XdKualT5QNGRWRWywe1d1lhNc9AL82ivhoge59v7"
#define WIFI_SSID "OPPO A92"
#define WIFI_PASSWORD "11111111"

// #define WIFI_SSID "ECO WIFI"
// #define WIFI_PASSWORD "11111111"

// pin SDA = D1 SCL = D2

LiquidCrystal_I2C lcd(0x27, 16, 2);
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

    lcd.begin(16, 2);
    lcd.init();
    lcd.backlight();

    dht.begin();

}
void loop() {
    String get_text = Firebase.getString("BinResponse/message");
    if (get_text){
      lcd.setCursor(0, 0);
      lcd.print("");
      lcd.print(0, 1);
      lcd.print(get_text);
      delay(1000);
    } 
    else{

      humidity = dht.readHumidity();
      temperature = dht.readTemperature();

      lcd.setCursor(0, 0);
      lcd.print("");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: " + humidity);
      // lcd.print("Humidity: ");
      // lcd.print(String(humidity));
      lcd.setCursor(0, 2);
      lcd.print("Temperature: " + temperature);
      // lcd.print("Temperature: ");
      // lcd.print(String(temperature));
      delay(800);
      lcd.clear();
    }

}