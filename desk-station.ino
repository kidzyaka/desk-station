#include <vector>
#include <string>


#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "FreeSansBold12pt7b.h"
#include "Org_01.h"
#include "Picopixel.h"


const char* ssid = "iPhone 5S";
const char* password = "gBvv43$!odNnm";

static const unsigned char PROGMEM image_Layer_9_bits[] = {0x38,0x7c,0xfe,0x6c,0x6c};

unsigned long loopTimer;


//finnhub token
// add &token= before your token
const char* apiKey = "&token=d6sg94pr01qj447bibvgd6sg94pr01qj447bic00";
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

  // display.clearDisplay();
  // display.fillRect(0, 52, 128, 15, SSD1306_WHITE);

  

  // if (WiFi.status() != WL_CONNECTED) {
  //   Serial.println("WiFi connection lost. Reconnecting...");
  //   WiFi.disconnect();
  //   WiFi.begin(ssid, password);
  // }

  // //clocks
  // display.setTextSize(2);
  // display.setCursor(33, 22);
  // display.setTextColor(SSD1306_WHITE);
  // display.println("00:00");

  // //weather
  // display.setTextSize(1);
  // display.setCursor(100,0);
  // display.print("22 C");

  // display.display();


  //WIP
  // need to migrate on millis

  

  drawScreen_1();
}



void drawScreen_1(void) {




  if((millis() - loopTimer) >= 50000){

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
        
          display.clearDisplay();

    }



    //render clocks
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setFont(&FreeSansBold12pt7b);
    display.setCursor(5, 44);
    display.print("20:33");

    display.drawRect(0, 53, 54, 11, 1);

    //render stocks
    display.setTextSize(1);
    display.setFont(&Org_01);
    display.setCursor(3, 60);
    display.print(stocks[i].c_str());


    display.setFont(&Picopixel);
    display.setCursor(29, 60);
    display.print(currentPrice);

    display.drawRect(109, 53, 19, 11, 1);

    //render temperatures
    display.setCursor(112, 60);
    display.print("22 C");

    display.drawRect(82, 53, 26, 11, 1);

    display.setCursor(92, 60);
    display.print("22 C");

    //render icons

    display.drawBitmap(84, 56, image_Layer_9_bits, 7, 5, 1);

    display.display();

  
      } else {
        Serial.println("http error");
      }
      http.end();
    }

  loopTimer = millis()
    
}