#include <ESP8266WiFi.h>  

//서버에 전송하기 위한 초기설정
const char* server = "api.thingspeak.com";
String apiKey = "Z4ANIE0Q33CDMA97";
const char* MY_SSID = "Gstep_9F";//와이파이 이름
const char* MY_PWD = "********";//와이파이 비밀번호

double Sum_water = 0;
volatile int NbTopsFan; 
int Calc;
int hallsensor = 5; 

void rpm () 
{
  NbTopsFan++; 
}

//// 초기 설정(시리얼 통신, 핀 설정)
void setup() {
  pinMode(hallsensor, INPUT);
  Serial.begin(115200);
  
  delay(10);

//와이파이 연결
  Serial.print("Connecting to " + *MY_SSID);

  WiFi.begin(MY_SSID, MY_PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");

  attachInterrupt(5, rpm, RISING);
}

void loop() {
  NbTopsFan = 0; 
  sei(); 
  delay (2000); 
  cli(); 
  Calc = (NbTopsFan / 5.5);

  Sum_water += ((double)Calc / 60) * 15;

  Serial.println("L/m : "+String(Calc)); 
  Serial.println("L : "+String(Sum_water));
  
  //Wifi 연결
  WiFiClient client;
 
  if (client.connect(server, 80)) { // 서버에 전송
    Serial.println("WiFi Client connected");

    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(Calc);
    postStr +="&field2=";
    postStr += String(Sum_water);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(3000);
  }
  Serial.println("Waiting...\n");
  delay(10000);
  client.stop();
}
