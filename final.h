#include secrets.h
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <Wire.h>

#define AWS_IOT_PUBLISH_TOPIC   outTopic
#define AWS_IOT_SUBSCRIBE_TOPIC esp32/sub

// Define the RPM sensor pin
int rpm_sensor = 15;

// Variables for tachometer logic
volatile unsigned long pulseCount = 0; // Variable to store pulse count
unsigned long startTime = millis(); // Stores start time for RPM calculation
const float revolutionsPerPulse = 1.0; // Adjust revolutions per pulse according to your setup

// Constants for voltage calculation
const float rpmToVoltageSlope = 12.0 / 2400.0; // Slope for linear interpolation
const float rpmToVoltageIntercept = 0.0; // Intercept for linear interpolation

// AWS IoT configuration
WiFiClientSecure net;
PubSubClient client(net);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, id.pool.ntp.org);

void connectAWS() {
  Serial.println(Connecting to Wi-Fi);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(.);
  }

  Serial.println(nWi-Fi connected);
  Serial.println(Connecting to AWS IoT);

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageHandler);

    Serial.print(.);
    if (client.connect(THINGNAME)) {
      Serial.println(nConnected to AWS IoT);
      client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    } else {
      Serial.println(nAWS IoT Connection Failed! Retrying...);
      delay(1000);
    }
  }
}

void publishRPMAndVoltage() {
  unsigned long elapsedTime = millis() - startTime;
  if (elapsedTime > 1000) { // Update RPM and voltage every second
    float rpm = (float)pulseCount * 60000.0 / elapsedTime * revolutionsPerPulse;
    float voltage = rpm * rpmToVoltageSlope + rpmToVoltageIntercept;
    pulseCount = 0; // Reset pulse count
    startTime = millis();

    StaticJsonDocument<200> doc;
    doc[rpm] = rpm;
    doc[voltage] = voltage;

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  }
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print(Incoming message from topic: );
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload, length);
  const char* message = doc[message];
  Serial.println(message);
}

void setup() {
  pinMode(rpm_sensor, INPUT);
  Serial.begin(115200);
  connectAWS();
  Wire.begin();
  timeClient.begin();
  timeClient.setTimeOffset(0);
  attachInterrupt(digitalPinToInterrupt(rpm_sensor), countPulse, RISING); // Attach interrupt to RPM sensor pin
}

void loop() {
  publishRPMAndVoltage();
  client.loop();

  Serial.print(RPM: );
  Serial.println(pulseCount);
}

void countPulse() {
  pulseCount++;
}

