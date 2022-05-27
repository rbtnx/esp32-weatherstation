#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Servo.h>
#include "credentials.h"

#define SERVO1_PIN 26
#define SERVO2_PIN 13

struct positionMap {
  const char* label;
  unsigned int angle;
};

// Adjust depending on your servo and print
const positionMap servoPos[] = {
  { "Clear", 0 },
  { "Clouds", 36 },
  { "Rain", 72 },
  { "Thunderstorm", 108 },
  { "Snow", 144 },
  { "Mist", 180 },
  { "Now", 0 },
  { "3hr", 36 },
  { "6hr", 72 },
  { "12hr", 108 },
  { "24hr", 144 },
  { "48hr", 180 }
};

Servo servoMotor1;
Servo servoMotor2;

String httpGETRequest(const char* url);
String weatherLookup(unsigned int id);

const char* ssid = SSID;
const char* password = PASS;

String owmApiKey = APIKEY;
String city = "Hamburg";
String countryCode = "DE";

unsigned long lastChecked = 0;
unsigned long timerDelay = 10000;

String jsonBuffer;

void setup() {
  servoMotor1.attach(SERVO1_PIN);
  servoMotor2.attach(SERVO2_PIN);

  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.print("Connecting...");
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to wifi with IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("First message in ");
  Serial.print(timerDelay/1000);
  Serial.println(" seconds.");
}


void loop() {
  unsigned int weatherID;

  if((millis() - lastChecked) > timerDelay) {
    if(WiFi.status() == WL_CONNECTED) {
      String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + owmApiKey;

      jsonBuffer = httpGETRequest(url.c_str());
      Serial.println(jsonBuffer);
      JSONVar jsonAnswer = JSON.parse(jsonBuffer);
      
      if(JSON.typeof(jsonAnswer) == "undefined") {
        Serial.println("Parsing went wrong! ");
        return;
      }
      Serial.print("JSON object = ");
      Serial.println(jsonAnswer);
      Serial.print("Temperature : ");
      Serial.println(jsonAnswer["main"]["temp"]);
      weatherID = int(jsonAnswer["weather"][0]["id"]);
      Serial.print("Weather id: ");
      Serial.print(weatherID);
      Serial.print(" = ");
      Serial.println(weatherLookup(weatherID));
    }
    else {
      Serial.println("Wifi disconnected!");
    }
    lastChecked = millis();
  }
}

String httpGETRequest(const char* url) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, url);
  int r = http.GET();

  String payload = "{}";

  if(r > 0) {
    Serial.print("HTTP response code: ");
    Serial.println(r);
    payload = http.getString();
  }
  else {
    Serial.print("GET error code: ");
    Serial.println(r);
  }
  http.end();
  
  return payload;
}

String weatherLookup(unsigned int id) {
  if(id >= 800 && id <= 802) {
    return "Clear";
  }
  if(id >= 803 && id <= 804) {
    return "Clouds";
  }
  if(id >= 701 && id <= 799) {
    return "Mist";
  }
  if(id >= 600 && id <= 622) {
    return "Snow";
  }
  if(id >= 500 && id <= 531) {
    return "Rain";
  }
  if(id >= 300 && id <= 321) {
    return "Rain";
  }
  if(id >= 200 && id <= 232) {
    return "Thunderstorm";
  }
  else {
    return "Mist";
  }
}

int 