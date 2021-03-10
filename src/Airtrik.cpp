#include "Airtrik.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* host = "airtrik.com"; 
String url = "/iot/";


void onReceive(char* deviceId, byte* payload, unsigned int length);

void callback(char* topic, byte* payload, unsigned int length) {

  int i=0, j=0, flag=0;
  for(i=0; i<strlen(topic); i++)
  {
      if(flag == 1)
      {
          j++;  
                      
      }
      else if(topic[i]=='/'){
          flag=1;
      }
  }
  char *mtopic = (char*)malloc(sizeof(char*)*j);
  i=0, j=0, flag=0;
  for(i=0; i<strlen(topic); i++)
  {
      if(flag == 1)
      {
         
          mtopic[j] = topic[i];    
          j++;    
      }
      else if(topic[i]=='/'){
          flag=1;
      }
  }
  mtopic[j]='\0';

  onReceive(mtopic, payload, length);
}
BearSSL::WiFiClientSecure wclient;
PubSubClient client(host, 8883, callback, wclient);


void Airtrik::send(const char* deviceId, const char* message){
  char * topic = (char *) malloc(1 + strlen(AIRTRIK_key)+ strlen("/") +strlen(deviceId) );
  strcpy(topic, AIRTRIK_key);
  strcat(topic, "/");
  strcat(topic, deviceId);
  client.publish(topic, message);
}

int Airtrik::subscribe(const char* deviceId){
  char * topic = (char *) malloc(1 + strlen(AIRTRIK_key)+ strlen("/") +strlen(deviceId) );
  strcpy(topic, AIRTRIK_key);
  strcat(topic, "/");
  strcat(topic, deviceId);
  return client.subscribe(topic);
  
}




void Airtrik::loop(){
  if (client.connected()){
    client.loop();
  }
}

Airtrik::Airtrik(){
  Serial.println("Importing AIRTRIK IOT CLOUD");
}


void Airtrik::connect(String ssid, String pass, char* key){

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  AIRTRIK_key = key;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    const int httpPort = 443;
    HTTPClient http;
    http.begin("https://airtrik.com/iot/", "01 e4 8f 2e bf 84 a8 f6 91 58 d4 02 ee c4 34 5c a1 d8 0a 9b");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postdata = "key=" + String(key);
    int httpCode = http.POST(postdata);
    
    Serial.println(httpCode);

  
    if(httpCode > 0){
      String payload = http.getString();
      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 130;
      DynamicJsonDocument res(capacity);
      
      deserializeJson(res, payload);
      
      username = res["username"];
      password = res["password"];

      wclient.setInsecure();
      if (!client.connected()) {
        Serial.println("Connecting to AIRTRIK");
        const char* cl = "rowdy";
        if (client.connect(cl, username, password)) {
          Serial.println("Connected to AIRTRIK");
          
        } else {
          Serial.println("Could not connect to AIRTRIK server");   
        }
      }
    }
  }
}
