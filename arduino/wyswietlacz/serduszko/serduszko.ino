#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM serduszko[] =
{ 0b00111100, 0b00111100,
  0b01111110, 0b01111110,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b01111111, 0b11111110,
  0b00111111, 0b11111100,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000011, 0b11000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000 };


void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.drawBitmap((SCREEN_WIDTH-16)/2,(SCREEN_HEIGHT-16)/2,serduszko,16,16,1);
  display.display();
  delay(2000);
}

void loop() {
  int i=0, j=0;
  bool prawo=true,gora=true;
  while(true){
    display.clearDisplay();
    display.drawBitmap((SCREEN_WIDTH-16)/2+i,(SCREEN_HEIGHT-16)/2+j,serduszko,16,16,1);
    display.display();
    if(i>=SCREEN_WIDTH/2-8){
      prawo=false;
    }
    if(i<=-SCREEN_WIDTH/2+8){
      prawo=true;
    }
    if(j>=SCREEN_HEIGHT/2-8){
      gora=false;
      if(prawo){
        i--;
      }
    }
    if(j<=-SCREEN_HEIGHT/2+8){
      gora=true;
      if(prawo){
        i--;
      }
    }
    if(prawo){
      i++;
    }
    else{
      i--;
    }
    if(gora){
      j++;
    }
    else{
      j--;
    }
    delay(10);
  }
}
