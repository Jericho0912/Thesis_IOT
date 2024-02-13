#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"

// Replace with your network credentials
const char* ssid     = "TP-Link_203C";
const char* password = "01686531";

// Define the pin where the LDR is connected
#define LDR_PIN 36

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variable to hold the LDR value
int LDR_Value = 0;

// Function to read the LDR value
void readLDR() {
  LDR_Value = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(LDR_Value);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // Create a server route to get the LDR value
server.on("/ldr", HTTP_GET, [](AsyncWebServerRequest *request){
  String json = "{\"ldr\":" + String(LDR_Value) + "}";
  request->send(200, "application/json", json);
});

server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Web Server: received a web page request");
    String html = HTML_CONTENT;  // Use the HTML content from the index.h file
    request->send(200, "text/html", html);
  });


  // Start server
  server.begin();
}

void loop(){
  readLDR();
  delay(1000);
}
