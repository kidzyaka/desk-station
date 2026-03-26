#include <vector>
#include <string>


#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "iPhone 5S";
const char* password = "gBvv43$!odNnm";

//finnhub token
// add &token= before your token
const char* apiKey = "";
//stocks
std::vector<std::string> stocks = {"AAPL",  "MSFT", "NVDA", "META"};


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  
  Wire.begin(21, 22);
  Serial.begin(115200);

  delay(200);
  display.setTextColor(SSD1306_WHITE);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("display OK");
  display.display();
  display.println("Connecting to WiFi...");
  display.display();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  display.println("WiFi connected");
  display.display();
  delay(500);
  display.clearDisplay();
  display.display();
}

void loop() {

  display.clearDisplay();
  display.fillRect(0, 52, 128, 15, SSD1306_WHITE);

  

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
  }

  //clocks
  display.setTextSize(2);
  display.setCursor(33, 22);
  display.setTextColor(SSD1306_WHITE);
  display.println("00:00");

  //weather
  display.setTextSize(1);
  display.setCursor(100,0);
  display.print("22 C");

  display.display();


  //WIP
  // need to migrate on millis

  for(int i = 0;i < stocks.size(); i++){

    
    
    HTTPClient http;
    
    String url = String("https://finnhub.io/api/v1/quote?symbol=") + stocks[i].c_str()+ apiKey;

    http.begin(url);


    
    int httpCode = http.GET();

      if (httpCode > 0) {
        String payload = http.getString();
      
        StaticJsonDocument<512> doc;
        deserializeJson(doc, payload);


        float currentPrice = doc["c"];
        float highPrice = doc["h"];
        
        display.setCursor(32, 22);

      
       

        //Stocks
        display.setTextSize(1);
        display.fillRect(0, 52, 128, 15, SSD1306_WHITE);
        display.setCursor(1, 55);
        display.setTextColor(SSD1306_INVERSE);
        display.print(stocks[i].c_str());
        display.setCursor(35, 55);

        display.print(currentPrice, SSD1306_INVERSE);
      
        display.setTextColor(SSD1306_INVERSE);
        Serial.println(currentPrice, SSD1306_INVERSE);
        display.display();
      } else {
        Serial.println("http error");
      }
      http.end();
      delay(10000);
    }
}