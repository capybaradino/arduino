#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "ConnectArduino";
const char *password = "capybara-san";

ESP8266WebServer server(80);

#define READY_LED_PIN 13

void handleRoot() {
    server.send(200, "text/html", "<h1>You are connected</h1>");
}

void LedOn(){
  server.send(200, "text/html", "<h1>LED is ON</h1>");
  digitalWrite(READY_LED_PIN,HIGH);
}

void LedOff(){
  server.send(200, "text/html", "<h1>LED is Off</h1>");
  digitalWrite(READY_LED_PIN,LOW);
}

void setup() {
  pinMode(READY_LED_PIN,OUTPUT);
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", handleRoot);
  server.on("/on/", LedOn);
  server.on("/off/", LedOff);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
