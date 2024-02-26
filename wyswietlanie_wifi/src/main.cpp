/*
Matrix LED 8x32
*/

//Declare libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
#include "EEPROM.h"
//#include <LEDMatrixDriver.hpp>
//Headers
//#include "font.h"
//#include "mat_functions.h"

#ifndef PSTR
#define PSTR  // Make Arduino Due happy
#endif

#define PIN 26

//provide your own WiFi SSID and password
const char* ssid = "317";
const char* password = "5L2dt7Qtjei7";
const String page PROGMEM = "<head>"
            " <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
            " <style>"
            " body{"
            " background-color:white;"
            " font-family: 'Roboto', sans-serif;" 
            "}"
            "</style>"
            " </head>"
            " <body>"
            " <h1>Wyswietlacz LCD</h1>"
            " <p>Wpisz cokolwiek do 32 znakow i kliknij ENTER, zeby przesc do nowej lini wpisz \",\"</p><p>\"turn off\" wylacza \"turn on\" wlacza"
            " <form action='/dataHandler' method='POST'>\r\n"
            " <label for='message'>Wiadomosc:</label> <input type='text' name='message' id='message'>""</p>\r\n" 
            " <input width='100' height='40' type='submit' id='send' value=' ENTER ' />\r\n "
            " </form> \r\n"
            "</body>";

//Create WebServer instance
WebServer server(80);

int pass = 0;
//Initialize message to display
String message = "";
int len = message.length();
int len2 = 30;
int len3 = len + len2;
LiquidCrystal_I2C lcd(0x27,16,2);  
void dataHandler();

void setup() {
  //This uses EEPROM to store previous message
  //Initialize EEPROM

  if (!EEPROM.begin(1000)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  //For debugging
  Serial.begin(9600);
  //Use ESP32 as WiFi Station
  WiFi.mode(WIFI_STA);
  //Initiate WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Print your WiFi's SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  //Print your local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());
  //Home page. Contents of 'page' is in mainpage.h
  server.on("/", []() {
    server.send(200, "text/html", page);
  });
  //POST data handler
  server.on("/dataHandler", HTTP_POST, dataHandler);
  //start web server
  server.begin();
  //Just stating things
  Serial.println("HTTP server started");
  //At first start, read previous message from EEPROM
  message = EEPROM.readString(0);
  int len = message.length();

  //Print stored message to serial port for checking
  Serial.print("Stored message: ");
  Serial.println(message);
  lcd.init();
  lcd.backlight();
  lcd.print(WiFi.localIP());
  delay(1000);
}

String pom;
bool wlacz=true;
//function for handling POST data

void dataHandler() {
  String msg = server.arg("message");  //message from POST data
  Serial.print("Received: ");          //print to serial terminal
  Serial.println(msg);
  //msg.toUpperCase();  //all incoming string is converted to uppercase since no font for small cases
  message = msg;
  EEPROM.writeString(0, message);      //store received message to EEPROM
  EEPROM.commit();                     //commit the save
  server.sendHeader("Location", "/");  //redirect client to home page
  server.send(303);                    //redirect http code
}

void loop() {
  int len = message.length(); 
  String wiadomosc=message;
  String wiadomosc2="";
  String wiadomosc1="";
  int i;
  if(wlacz){
    pom=message;
  }
  for(i=len-1;i>0;i--){
    if(wiadomosc[i]==','){
      break;
    }
    wiadomosc2= wiadomosc[i]+wiadomosc2;
  }
  if(i==0){
    wiadomosc1=wiadomosc;
  }
  else{
    for(int j=0;j<i;j++){
      wiadomosc1=wiadomosc1+wiadomosc[j];
    }
  }
      //*************************************************na wypadek rozlaczenia
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(5000);
    Serial.print(".");
  if(WiFi.status() == WL_CONNECTED){
    lcd.clear();
    lcd.backlight();
    lcd.print("polaczono");
    delay(2000);
    lcd.clear();
    lcd.print(WiFi.localIP());
  }
  }
      //*************************************************obsługa wyswietlania

      if (message!=pom && !wlacz){
        if( message == "turn off"){
          lcd.clear();
          lcd.noBacklight();
          pom=message;
        }else{
          if(message=="turn on"){
            lcd.clear();
            lcd.backlight();
            pom=message;
          }
          else{
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(wiadomosc1);
            lcd.setCursor(0,1);
            if(i!=0){
              lcd.print(wiadomosc2);
            }
            pom=message;
          }
        }
      }
      server.handleClient();
      wlacz=false;
}