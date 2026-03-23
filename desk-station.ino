#include <vector>
#include <string>


#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "frasnkensh";
const char* password = "gBvv43$!odNnm";

//finnhub token
// add &token= before your token
const char* apiKey = "";
//stocks
std::vector<std::string> stocks = {"AAPL", "DSCD", "MCFT"};


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Wire.begin(21, 22);
  Serial.begin(115200);

  delay(200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
  }

  display.clearDisplay();

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("wifi connected");
  display.display();
  delay(500);
  display.clearDisplay();
  display.display();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
  }


  //WIP
  HTTPClient http;
    
    String url = String("https://finnhub.io/api/v1/quote?symbol=AAPL") + apiKey;

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      
      StaticJsonDocument<512> doc;
      deserializeJson(doc, payload);

      float currentPrice = doc["c"];
      float highPrice = doc["h"];
      display.setTextSize(2);
      display.clearDisplay();
      display.setCursor(44, 15);
      display.print("AAPL");
      display.setCursor(33, 35);
      display.print(currentPrice);
      

      Serial.println(currentPrice);
      display.display();
    } else {
      Serial.println("http error");
    }
    http.end();


  delay(10000); 
}