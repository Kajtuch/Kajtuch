#include <VirtualWire.h>
 
// definiujemy piny 
#define potencjometr 1
#define button_pin 5
#define transmit_pin 2
 
void setup()
{
    pinMode(6,OUTPUT);
    digitalWrite(6,HIGH);
  // przygotowujemy potrzebne informacje dla biblioteki
    vw_set_tx_pin(transmit_pin);
    vw_setup(2000);
    pinMode(button_pin, INPUT);
    Serial.begin(9600);
}
 
void loop()
{
  int napiecie=analogRead(potencjometr);
  if(true){
  String toSend = int_to_string(napiecie) ; // tekst wiadomości
  char msg[23]; // tworzymy tablicę typu char
  toSend.toCharArray(msg, toSend.length() + 1); // konwertujemy nasz tekst do tablicy char'ów
  vw_send((uint8_t *)msg, strlen(msg));// wysyłamy 
  vw_wait_tx(); 
  Serial.print(napiecie);
  delay(10); // czekamy 1 sekundę
  }
}

String int_to_string(int val){
  String string_val="";
  for(int i=0;val>0;i++){
    string_val=val%10+string_val;
    val=val/10;
  }
  return(string_val);
}