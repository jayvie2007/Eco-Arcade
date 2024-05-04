#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define DHTPIN D5

int is_switch_press = 0; // For detecting the switch press status
int debounce_delay = 300; //Debounce delay
int switch_button = D7;
 
float humidity;
float temperature;
DHT dht(DHTPIN, DHTTYPE);

//Setting up firebase and wifi connection
#define FIREBASE_HOST "eco-arcade-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XdKualT5QNGRWRWywe1d1lhNc9AL82ivhoge59v7"
// #define WIFI_SSID "OPPO A92"
// #define WIFI_PASSWORD "11111111"

#define WIFI_SSID "Smartbro-9EC6"
#define WIFI_PASSWORD "smartbro"

// pin SDA = D1 SCL = D2

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
    Serial.begin(9600);

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
    pinMode(switch_button, INPUT_PULLUP);

}
void loop() {
    is_switch_press = digitalRead(switch_button); // Reading the Switch press status

    String line1 = Firebase.getString("Printer/Line1");
    int line2 = Firebase.getInt("Printer/Line2");
    int line3 = Firebase.getInt("Printer/Line3");
    int line4 = Firebase.getInt("Printer/Line4");
    String line5 = Firebase.getString("Printer/Line5");
    String printer_starter = Firebase.getString("Printer/start");
    // String datetime_now;
    if (is_switch_press == LOW){
        delay(debounce_delay); // debounce delay for button press
        lcd.setCursor(0, 0);
        lcd.print("                    ");
        lcd.setCursor(0, 1);
        lcd.print("Please Wait....");
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        delay(800);
        if (printer_starter == "start"){
          delay(2000);
          lcd.setCursor(0, 0);
          lcd.print("                    ");
          lcd.setCursor(0, 1);
          lcd.print("Printing....");
          lcd.setCursor(0, 2);
          lcd.print("                    ");
          Serial.println(line1);
          Serial.println("---------------------------- \n\r");
          Serial.println("\n\r");
          Serial.println("\n\r");
          Serial.println("\n\r");
          delay(400);
          Serial.print("Can = ");
          Serial.println(line2);
          Serial.print("Plastic = ");
          Serial.println(line3);
          Serial.print("Paper = ");
          Serial.println(line4);
          delay(400);
          Serial.println("\n\r");
          Serial.println("\n\r");
          Serial.println("---------------------------- \n\r");
          Serial.println(line5);
          Serial.println("\n\r");
          Serial.println("Present this to");
          Serial.println("Submit your points.");
          Serial.println("\n\r");
          Serial.println("\n\r");
          Serial.println("\n\r");
          Firebase.setString("Printer/start", "stop");
          Firebase.setInt("Printer/Line2", 0);
          Firebase.setInt("Printer/Line3", 0);
          Firebase.setInt("Printer/Line4", 0);
          delay(1000);
        } else{ 
            lcd.setCursor(0, 0);
            lcd.print("                    ");
            lcd.setCursor(0, 1);
            lcd.print("Insert bottle first");
            lcd.setCursor(0, 2);
            lcd.print("                    ");
            delay(1000);
        }
    } else{
        String get_text = Firebase.getString("BinResponse/message");
        if(get_text != ""){
            lcd.setCursor(0, 0);
            lcd.print("                    ");
            lcd.setCursor(0, 1);
            lcd.print(get_text);
            lcd.setCursor(0, 2);
            lcd.print("                    ");
            delay(800);
        } else{
            humidity = dht.readHumidity();
            temperature = dht.readTemperature();
            Firebase.setInt("Weather/humidity", humidity);
            Firebase.setInt("Weather/temperature", temperature);

            lcd.setCursor(0, 0);
            lcd.print("Humidity: " + String(humidity));
            lcd.setCursor(0, 1);
            lcd.print("                    ");
            lcd.setCursor(0, 2);
            lcd.print("Temperature: " + String(temperature));
            delay(800);
        }
  }
}