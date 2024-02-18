#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  // Additional setup code
}
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("AKI");
  delay(1000);

  lcd.setCursor(0, 1);
  lcd.print("");
  delay(800);

  lcd.clear();
}