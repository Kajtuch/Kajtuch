#include <Wire.h> 
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

const String WIADOMOSC = "kajtek";

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


uint8_t heart0[8] = {
0b0,
0b01010,
0b11111,
0b11111,
0b01110,
0b00100,
0b0};
uint8_t heart1[8] = {
0b01010,
0b11111,
0b11111,
0b01110,
0b00100,
0b0,
0b0};



void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
  lcd.createChar(0,heart0);
  lcd.createChar(1,heart1);
}

void loop()
{



  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.printByte(1);
  lcd.print(WIADOMOSC);
  lcd.setCursor(WIADOMOSC.length()+1,0);
  lcd.printByte(1);

  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.printByte(0);
  lcd.print(WIADOMOSC);
  lcd.setCursor(WIADOMOSC.length()+1,0);
  lcd.printByte(0);
}