/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
struct piksel{
  int x;
  int y;
} pozycja,ogon[40],japko;

char dec_to_string(int* punkty);
void pokarz_sneka(piksel pozycja , piksel* ogon,int dlugosc);
int mapa[128][46];
int dlugosc=3;

void reset( int*dlugosc, piksel* pozycja, piksel* ogon){
  (*pozycja).x=20;
  (*pozycja).y=39;
  ogon[0].x=-10;
  ogon[0].y=-10;
  ogon[1].x=-10;
  ogon[1].y=-10;
  ogon[2].x=-10;
  ogon[2].y=-10;
  *dlugosc=3;
  japko.x=rand()%125;
  japko.y=rand()%44+17;
}

void sciany(){
  for(int i=0;i<128;i++){
    display.drawPixel(i,17,1);
    display.drawPixel(i,63,1);
  }
  for(int i=17;i<64;i++){
    display.drawPixel(0,i,1);
    display.drawPixel(127,i,1);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pozycja.x=20;
  pozycja.y=39;
  ogon[0].x=20;
  ogon[0].y=39;
  ogon[1].x=20;
  ogon[1].y=39;
  ogon[2].x=20;
  ogon[2].y=39;
  japko.x=rand()%125;
  japko.y=rand()%44+17;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.clearDisplay();
  display.display();

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();

}

void sprawdz_pozycje(int* dlugosc, piksel* pozycja, piksel* ogon){
  for(int i=0;i<*dlugosc;i++){
    if((*pozycja).x==ogon[i].x && (*pozycja).y==ogon[i].y){
      reset(dlugosc,pozycja,ogon);
      break;
    }
  }
  if(((*pozycja).x==japko.x || (*pozycja).x==japko.x+1 || (*pozycja).x==japko.x+2 || (*pozycja).x==japko.x-1 || (*pozycja).x==japko.x-2 )&&( (*pozycja).y==japko.y || (*pozycja).y==japko.y+1 || (*pozycja).y==japko.y+2 || (*pozycja).y==japko.y-1 || (*pozycja).y==japko.y-2)){
    (*dlugosc)++;
    japko.x=rand()%125;
    japko.y=rand()%44+17;
  }
}

void wypisz_punkty(int punkty){
  String napis="PUNKTY=",ilosc="";
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(0,0);
  display.cp437(1);
  if(punkty==0){
    ilosc='0';
  }
  else{
   while(punkty){
    ilosc=dec_to_string(&punkty)+ilosc;
   }
  }
  napis=napis+ilosc;
  for(int j=0;j<napis.length();j++){
    display.write(napis[j]);
  }
  
   
}
char dec_to_string(int* punkty){
  int pom=*punkty%10;
  *punkty=*punkty/10;
  return (char)('0'+pom);
}

void pokarz_sneka(piksel pozycja,piksel* ogon,int dlugosc){
  display.drawPixel(pozycja.x, pozycja.y, 1);
  display.drawPixel(pozycja.x+1, pozycja.y, 1);
  display.drawPixel(pozycja.x+2, pozycja.y, 1);
  display.drawPixel(pozycja.x, pozycja.y+1, 1);
  display.drawPixel(pozycja.x+1, pozycja.y+1, 1);
  display.drawPixel(pozycja.x+2, pozycja.y+1, 1);
  display.drawPixel(pozycja.x, pozycja.y+2, 1);
  display.drawPixel(pozycja.x+1, pozycja.y+2, 1);
  display.drawPixel(pozycja.x+2, pozycja.y+2, 1);
  int j=0;
  while(j!=dlugosc){
    for(int i=0;i<3;i++){
      for(int h=0;h<3;h++){
        display.drawPixel(ogon[j].x+i, ogon[j].y+h, 1);
      }
    }
    j++;
  }
  for(int i=0;i<3;i++){
      for(int h=0;h<3;h++){
        display.drawPixel(japko.x+i, japko.y+h, 1);
      }
    }
  

}
void przesuwanie_ogona(piksel* ogon){
  for(int j=39;j>0;j--){
    ogon[j]=ogon[j-1];
  }
  ogon[0]=pozycja;
}

int kierunek=1,time=0;
bool odklik = false;
//***********************************************8
void loop() {
  display.clearDisplay();
  wypisz_punkty(dlugosc-3);
  pokarz_sneka(pozycja,ogon,dlugosc);
  sciany();
  if(!digitalRead(3)){
    kierunek++;
    if(odklik){
      kierunek--;
    }
    odklik=true;
  }
  else{
    if(!digitalRead(2)){
      kierunek--;
      if(odklik){
      kierunek++;
    }
      odklik=true;
    }
    else{
      odklik=false;
    }
  }

  if(kierunek>3){
    kierunek=0;
  }
  if(kierunek<0){
    kierunek=3;
  }
 if(time==10){
  time=0;
  switch (kierunek){
    case 0:{
      if(pozycja.y==18){
        reset(&dlugosc,&pozycja,ogon);
      }
      else{
        pozycja.y=pozycja.y-3;
      }
      break;
    }
    case 1:{
      if(pozycja.x==125){
        reset(&dlugosc,&pozycja,ogon);
      }
      else{
       pozycja.x=pozycja.x+3;
      }
      break;
    }
    case 2:{
      if(pozycja.y==60){
        reset(&dlugosc,&pozycja,ogon);
      }
      else{
        pozycja.y=pozycja.y+3;
      }
      
      break;
    }
    case 3:{
      if(pozycja.x==-1){
        reset(&dlugosc,&pozycja,ogon);
      }
      else{
        pozycja.x=pozycja.x-3;
      }
      break;
    }
  }
  sprawdz_pozycje(&dlugosc,&pozycja,ogon);
  przesuwanie_ogona(ogon);
 }
  


  sciany();
  display.display();
  time++;
  delay(1);
}

