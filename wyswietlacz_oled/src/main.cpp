#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WiFi.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define WIADOMOSC "KOCHAM JULKE"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "pokój 317";
const char* password = "grubas123";
WiFiServer server(80);

void wypisz_cos(String napis);

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid,password);
  Serial.println("łącze się z wifi");
  Serial.println("czekaj");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("połączono!");
  Serial.print("IP:");
  Serial.print(WiFi.localIP());
  server.begin();


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(2000);
}

void loop() {
   WiFiClient client = server.available();   // listen for incoming clients
 
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
 
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
 
            // the content of the HTTP response follows the header:
            client.print("<head><title>Wyswietlacz</title>  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <style>.button1{ border: 1px; padding: 15px 32px; font-size: 16px; text-align: center; }.zie {background-color: #04AA6D;} .czer {background-color: tomato;} </style> </head>");
            client.print("<h1>wlacz/wylacz napis</h1>");
            client.print("<a href=\"/H\"> <button class=\"button1 zie\"> ON </button> </a> <br>");
            client.print("<a href=\"/L\"> <button class=\"button1 czer\"> OFF </button> </a> <br>");
 
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
 
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          wypisz_cos(WIADOMOSC);
        }
        if (currentLine.endsWith("GET /L")) {
          display.clearDisplay();              // GET /L turns the LED off
          display.display();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void wypisz_cos(String napis){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(0,0);
  display.cp437(1);

  for(int i=0;i<napis.length();i++){
    if(napis[i]==' '){
      display.setCursor(0,16);
    }
    else{
      display.write(napis[i]);
    }
  }
  display.display();
}

