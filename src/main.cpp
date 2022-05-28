/* 
 * Author: Kathrin Weihe, 2022
 */
 
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <ESP32Servo.h>
#include <wstation.h>
#include "credentials.h"

#define SERVO1_PIN 26
#define SERVO2_PIN 13

String httpGETRequest(const char* url);

Servo servoMotor1;
Servo servoMotor2;

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
  unsigned int servo1Angle;

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
      servo1Angle = weatherLookup(weatherID);
      Serial.print("Weather id: ");
      Serial.print(weatherID);
      Serial.print(" Angle: ");
      Serial.print(servo1Angle);
    }
    else {
      Serial.println("Wifi disconnected!");
    }
    lastChecked = millis();
  }
  servoMotor1.write(servo1Angle);
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