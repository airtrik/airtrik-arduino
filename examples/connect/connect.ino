#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "airtrik.h"

char* mykey = "__APP_KEY__";
String ssid = "WIFI__NAME";
String pass = "WIFI__PASSWORD";

void onReceive(char* deviceId, byte* message, unsigned int length){
  int i;
  for(i=0; i < length; i++){
    Serial.print(char(message[i]));
  }
  Serial.println("");
  Serial.println(deviceId);
  
}

airtrik iot;

void setup() {
  iot.connect(ssid, pass, mykey);
  iot.subscribe("__DEVICE_ID__");
}

void loop() {
  const char* msg = "__MESSAGE__";
  const char* deviceId = "__DEVICE_ID__";
  iot.send(deviceId, msg);
  iot.loop();
  delay(1000);
}
