#include <WiFi.h>


const char* ssid = "CyberRange"; 
const char* password = "CyberRange";


void setupWiFi() {
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}



