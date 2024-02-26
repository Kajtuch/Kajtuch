#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h" 

RF24 radio(7,8);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
unsigned long Y; 
unsigned long X; 
unsigned long predkosc;
bool przycisk_1,przycisk_2;

void setup(){
pinMode(2,INPUT);
pinMode(3,INPUT);
Serial.begin(9600);
//to co się tutaj dzieje jest potrzebne do działania nrfu
radio.begin();

radio.setRetries(15,15);

radio.openReadingPipe(1,pipes[1]);

radio.startListening();

radio.printDetails();

radio.openWritingPipe(pipes[0]);

radio.openReadingPipe(1,pipes[1]);

radio.stopListening(); 
}

void loop() {
//nasz pakiet danych to 14 znakowy string w którym kolejne bity oznaczają wartość prędkości wychylenia joysticków i przyciski (narazie są dwa zaimplementowane)

delay(10);
//zaczytywanie przycisków
bool p1=false, p2=false;
if(digitalRead(2)){
  p1=true;
}
if(digitalRead(3)){
  p2=true;
}

//****************************** do debugowania
// Serial.print("\nprzycisk 1: ");
// Serial.print(p1);
// Serial.print("\nprzycisk 2:");
// Serial.print(p2);
// Serial.print("\n os X: ");
// Serial.print(analogRead(7));
// Serial.print("\n os Y: ");
// Serial.print(analogRead(6));
// Serial.print("\npredkosc: ");
// Serial.println(analogRead(1));
//******************************

przycisk_1=p1;
przycisk_2=p2;
//zczytywanie wartosci analogowych
X=analogRead(7);
Y=analogRead(6);
predkosc=analogRead(1);
char wiadomosc[14];
String feedback="";
tworzenie_wiadomosci(Y,X,predkosc,p1,p2,wiadomosc);

 for(int i=0;i<14;i++){
   feedback=feedback+wiadomosc[i];
 }

 Serial.println(feedback);
//wysyłanie
radio.stopListening();
radio.write( wiadomosc, sizeof(char[14]) );
radio.startListening(); 


}

 void tworzenie_wiadomosci(unsigned long Y,unsigned long X,unsigned long P, bool p1, bool p2, char *wiadomosc){//
   int pom=0;
  for(int i=0;i<14;i++){
    wiadomosc[i]=(char)'0';
  }
  for(int i=0;i<4;i++){
    pom=Y%10;
    wiadomosc[3-i]=((char)(pom+'0'));
    Y=Y/10;
  }
  for(int i=0;i<4;i++){
    pom=X%10;
    wiadomosc[7-i]=(char)(pom+'0');
    X=X/10;
  }
  for(int i=0;i<4;i++){
    pom=P%10;
    wiadomosc[11-i]=(char)(pom+'0');
    P=P/10;
  }
  if(p1){
    wiadomosc[12]=(char)'1';
  }
  else{
    wiadomosc[12]=(char)'0';
  }
  if(p2){
    wiadomosc[13]=(char)'1';
  }
  else{
    wiadomosc[13]=(char)'0';
  }
}






