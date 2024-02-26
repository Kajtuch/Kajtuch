 #include <SPI.h>

#include "nRF24L01.h"

#include "RF24.h"


RF24 radio(7,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef enum { role_ping_out = 1, role_pong_back } role_e;

const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

role_e role = role_pong_back;

void setup(void)

{

Serial.begin(9600);

radio.begin();

radio.setRetries(15,15);

radio.openReadingPipe(1,pipes[1]);

radio.startListening();

radio.printDetails();

radio.openWritingPipe(pipes[1]);

radio.openReadingPipe(1,pipes[0]);

radio.startListening();

pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(8,OUTPUT);
stop();

}

int swiatla=0;
void loop(void)

{


if ( radio.available() )

{

unsigned long Y=0;
unsigned long X=0;
unsigned long predkosc=0;
bool przycisk1 = 0;
bool przycisk2 = 0;
char wiadomosc[14];
String feedback="";
radio.read(wiadomosc,sizeof(char[14]));

for(int i=0;i<14;i++){
 feedback=feedback+wiadomosc[i];
}
dekoder(wiadomosc,&Y,&X,&predkosc,&przycisk1,&przycisk2);

Serial.println(feedback);
Serial.print("\nY: ");
Serial.print(Y);
Serial.print("\nX: ");
Serial.print(X);
Serial.print("\npredkosc: ");
Serial.print(((double)(predkosc)/506)*100);
Serial.print("%");
Serial.print("\nprzycisk1: ");
Serial.print(przycisk1);
Serial.print("\nprzycisk2: ");
Serial.println(przycisk2);

if(Y==0){
  przod();
}
else{
  if(Y>=1000){
    tyl();
  }
  else{
    if(X==0){
      prawo();
    }
    else{
      if(X>=1000){
        lewo();
      }
      else{
        stop();
      }
    }
  }
}
switch(swiatla){
  case 0:{
    if(przycisk1){
      swiatla=1;
      digitalWrite(8,HIGH);
      break;
    }
    else{
      break;
    }
  }
  case 1:{
    if(przycisk1){
      break;
    }
    else{
      swiatla=2;
      break;
    }
  }
  case 2:{
    if(przycisk1){
      swiatla=3;
      digitalWrite(8,LOW);
      break;
    }
    else{
      break;
    }
  }
  case 3:{
    if(przycisk1){
      break;
    }
    else{
      swiatla=0;
      break;
    }
  }
}
}

} 

void dekoder(char *wiadomosc,unsigned long *Y,unsigned long* X,unsigned long* pr,bool *p1,bool *p2){
  int i=0;
  while(i<4){
    *Y+=((int)wiadomosc[i]-'0')*pow(10,3-i);
    i++;
  }
    while(i<8){
    *X+=((int)wiadomosc[i]-'0')*pow(10,7-i);
    i++;
  }
  while(i<12){
    *pr+=((int)wiadomosc[i]-'0')*pow(10,11-i);
    i++;
  }
  if((int)wiadomosc[i]-'0'){
    *p1=true;
  }
  else{
    *p1=false;
  }
  if((int)wiadomosc[i+1]-'0'){
    *p2=true;
  }
  else{
    *p2=false;
  }
}
void stop(){
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
}
void tyl(){
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
}
void lewo(){
  digitalWrite(2,HIGH);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
}
void prawo(){
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
}
void przod(){
  digitalWrite(2,HIGH);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
}


