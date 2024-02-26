#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
// put function declarations here:
const char* ssid = "317";
const char* password = "5L2dt7Qtjei7";
LiquidCrystal_I2C lcd(0x27,16,2);  
HTTPClient client;
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println( "Lacze z WiFi: " + (String)ssid );
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("polaczono");
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.print("jest ok");
  client.begin("https://danepubliczne.imgw.pl/api/data/synop/id/12155");
}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int status = client.GET();
    if(status>0){
      lcd.clear();
      Serial.println("status: " + (String)status);
      //Serial.println(status);
      String dane = client.getString();
      //Serial.println(dane);
      char json[500];
      dane.toCharArray(json,500);
      StaticJsonDocument<200> doc;
      deserializeJson(doc,json);
      float temperatura = doc["temperatura"];
      Serial.println("temperatura "+(String)temperatura);
      lcd.print("temperatura:");
      lcd.setCursor(0,1);
      lcd.print((String)(temperatura)+(char)(0b11011111)+"C");
      delay(8000);
      int wiatr = doc["predkosc_wiatru"];
      lcd.clear();
      lcd.print("predkosc wiatru:");
      lcd.setCursor(0,1);
      lcd.print((String)wiatr+"km/h");
      delay(3000);
      float wilgotnosc = doc["wilgotnosc_wzgledna"];
      lcd.clear();
      lcd.print("wilgotnosc wzgl:");
      lcd.setCursor(0,1);
      lcd.print((String)wilgotnosc+(char)(0b00100101));
      delay(3000);
      float cisnienie = doc["cisnienie"];
      lcd.clear();
      lcd.print("cisninie:");
      lcd.setCursor(0,1);
      lcd.print((String)cisnienie+"hPa");
      delay(3000);
      float godzina = doc["godzina_pomiaru"];
      lcd.clear();
      lcd.print("godzina pomiaru:");
      lcd.setCursor(0,1);
      lcd.print((String)godzina);
      delay(3000);
      

    }
  }
  else{
    lcd.clear();
    lcd.print("lacze z siecia...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    lcd.print("polaczono");
  }
}