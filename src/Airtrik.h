#ifndef AIRTRIK_H
#define AIRTRIK_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

class Airtrik
{
	String url = "/iot/";
	const char* AIRTRIK_key = "";

	const char* username = "";
	const char* password = "";
  
	public:
		Airtrik();
		void loop();
		int subscribe(const char* deviceId);
		void send(const char* deviceId, const char* message);
    void connect(String ssid, String pass, char* key);
};


// void AIRTRIK_connect(String ssid, String pass, char* key);
// void AIRTRIK_send(const char* deviceId, const char* message);
// int AIRTRIK_subscribe(const char* deviceId);
void onReceive(char* deviceId, char* message);
// void AIRTRIK_loop();

#endif
