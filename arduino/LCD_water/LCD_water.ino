#include <Arduino.h>
#include <TM1637Display.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

char ssid[] = "Gstep_9F";          //  your network SSID (name) 
char pass[] = "********";   // your network password

int status = WL_IDLE_STATUS;
WiFiClient  client;
int Sum_water = 0;
    
unsigned long myChannelNumber = 438673; // modify this with your own Channel Number
const char * myReadAPIKey = "0YCKZRVB1AGIWNTK";

#define CLK 2
#define DIO 0

TM1637Display display(CLK, DIO);

void setup() {
  display.setBrightness(16);//밝기 설정 범위는 0 ~ 16
  
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  display.setSegments(data);//배열 출력
  delay(1000);
  
  Serial.begin(9600); // for debugging reasons
  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);
}

void loop() {
  Sum_water = ThingSpeak.readIntField(myChannelNumber, 2, myReadAPIKey);
  Serial.print("L : ");
  Serial.println(Sum_water);
  display.showNumberDec(Sum_water, false, 3, 1);
}
