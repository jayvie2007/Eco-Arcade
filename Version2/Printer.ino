#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <ESP8266HTTPClient.h>

#define DHTTYPE DHT22
#define DHTPIN D5

int is_switch_press = 0; // For detecting the switch press status
int debounce_delay = 300; //Debounce delay
int switch_button = D4;

float humidity;
float temperature;
DHT dht(DHTPIN, DHTTYPE);

int year;
int month;
int day;

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
    pinMode(switch_button, INPUT_PULLUP);
}
void loop() {
    is_switch_press = digitalRead(switch_button); // Reading the Switch press status
    
    String line1 = Firebase.getString("Printer/Line1");
    int line2 = Firebase.getInt("Printer/Line2");
    int line3 = Firebase.getInt("Printer/Line3");
    int line4 = Firebase.getInt("Printer/Line4");
    String line5 = Firebase.getString("Printer/Line5");
    String printer_starter = Firebase.getString("Printer/Start");
    String datetime_now;
    if (is_switch_press == LOW){
        delay(debounce_delay); // debounce delay for button press
        Serial.println(is_switch_press);
        lcd.setCursor(0, 0);
        lcd.print("                    ");
        lcd.setCursor(0, 1);
        lcd.print("Please Wait....");
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        delay(800);
        if (printer_starter == "start"){
          HTTPClient http;
          http.begin("http://worldtimeapi.org/api/ip"); // You can replace "ip" with the timezone if you prefer
          int httpResponseCode = http.GET();

          if (httpResponseCode > 0) {
              String payload = http.getString();
              DynamicJsonDocument doc(1024);
              deserializeJson(doc, payload);

              // Extract date components
              year = doc["year"];
              month = doc["month"];
              day = doc["day"];
              datetime_now = ("Date Printed: "+String(month)+"-"+String(day)+"-"+String(year))
              // Close the connection
              http.end();
          } else {
              Serial.print("Error getting date: ");
              Serial.println(httpResponseCode);
              lcd.setCursor(0, 0);
              lcd.print("                    ");
              lcd.setCursor(0, 1);
              lcd.print("Push button again...");
              lcd.setCursor(0, 2);
              lcd.print("                    ");
              delay(800);
              return
          }
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
          Serial.println("Present this to submit your points.");
          Serial.println(datetime_now);
          Serial.println("\n\r");
          Serial.println("\n\r");
          Serial.println("\n\r");
          Firebase.setString("Printer/start", "stop");
          Firebase.setInt("Printer/Line2", 0);
          Firebase.setInt("Printer/Line3", 0);
          Firebase.setInt("Printer/Line4", 0);
          delay(1000);
        
        } else{ 
            Firebase.setString("BinResponse/Message","Insert a bottle first");
            delay(1000);
            Firebase.setString("BinResponse/Message","");
        }
    } else{
        String get_text = Firebase.getString("BinResponse/message");
        Serial.println(get_text);

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

        Serial.println("humidity");
        Serial.println(humidity);
        Serial.println("temperature");
        Serial.println(temperature);

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