#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define CLIENT_MODE

#ifdef CLIENT_MODE
#include <Arduino.h>
#include "WROOM02.h"
//WiFiServer server(80);
//WiFiClient client;

#else
//#include <WiFiClient.h> 
const char *ssid = "ConnectArduino";
const char *password = "capybara-san";

#endif

ESP8266WebServer server(80);


#define READY_LED_PIN 13
#define LEDDELAY_WHEN_CONNECTED 250
//#define SOLENOID_PIN 14
#define SOLENOID_PIN 4
#define SOLENOID_DELAY 1000

void handleRoot() {
//    server.send(200, "text/html", "<h1>You are connected</h1>");
    String msg = "<html><head><title>ESP8266WebSerber</title></head><body><h1>You are connected</h1><h2><a href='/on/'>LED ON</a></h2><h2><a href='/off/'>LED OFF</a></h2><h2><a href='/solenoid/'>SOLENOID MOVE</a></h2></body></html>";
    server.send(200, "text/html", msg);
}

void LedOn(){
  server.send(200, "text/html", "<h1>LED is ON</h1>");
  digitalWrite(READY_LED_PIN,HIGH);
}

void LedOff(){
  server.send(200, "text/html", "<h1>LED is Off</h1>");
  digitalWrite(READY_LED_PIN,LOW);
}

void Solenoid(){
  server.send(200, "text/html", "<h1>Solenoid is moved</h1>");
  digitalWrite(SOLENOID_PIN,HIGH);
  delay(SOLENOID_DELAY);
  digitalWrite(SOLENOID_PIN,LOW);
}

void setup() {
  pinMode(READY_LED_PIN,OUTPUT);
  digitalWrite(READY_LED_PIN,LOW);
  pinMode(SOLENOID_PIN,OUTPUT);
  digitalWrite(SOLENOID_PIN,LOW);
    delay(1000);
    Serial.begin(115200);
    Serial.println();

#ifdef CLIENT_MODE

 Serial.println("");
  Serial.println("Start");
  Serial.print("Connecting to ");
  Serial.println(ssid);

// Connect to WiFi network
    WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(3000);  //Needed for ?
#else  
    Serial.print("Configuring access point...");
    WiFi.mode(WIFI_AP);
//    WiFi.softAP(ssid, password);
    WiFi.softAP(ssid, password, 1, 1);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
#endif    

    server.on("/", handleRoot);
  server.on("/on/", LedOn);
  server.on("/off/", LedOff);
  server.on("/solenoid/", Solenoid);
    server.begin();
    Serial.println("HTTP server started");

          for(int i=0; i<2; i++)
      {
        digitalWrite(READY_LED_PIN, HIGH);
        delay(LEDDELAY_WHEN_CONNECTED);
        digitalWrite(READY_LED_PIN, LOW);      
        delay(LEDDELAY_WHEN_CONNECTED);
      }
}

boolean isConnected=false;
void loop() {
    server.handleClient();
      if (!isConnected && WiFi.status())
      {
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
      delay(5000);
    Serial.println(WiFi.localIP());
    isConnected=true;
      }

}
