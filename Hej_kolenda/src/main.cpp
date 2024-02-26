//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int ilosc_wersow=16;
String tekst[ilosc_wersow]{
  "Bog sie rodzi","moc truchleje","Pan niebiosow","obnazony","Ogien krzepnie","blask tezeje","Ma granice","Nieskonczony","Wzgardzony","okryty chwala","Smiertelny","Bog nad wiekami","A slowo cialem","sie stalo","I mieszkalo","miedzy nami"
};

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  
}


void loop()
{
  delay(2000);
  for(int i=0;i<ilosc_wersow;i++){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(tekst[i]);
    delay(1500);
    i++;
    lcd.setCursor(0,1);
    lcd.print(tekst[i]);
    delay(2000);
  }
}