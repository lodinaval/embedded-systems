#include <ThingSpeak.h>
#include <WiFi.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT_MS 15000
#define CHANNEL_ID 3361750
#define API_KEY "YOUR_THINGSPEAK_WRITE_API_KEY"

WiFiClient client;
unsigned long lastUploadTime = 0;
int currentSoil = 0;
int currentPump = 0;

void connectToWifi(){
  Serial.println("Connecting to wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long startAttemptTime = millis();
  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);
  }
  if(WiFi.status() != WL_CONNECTED) Serial.println("Failed!");
  else Serial.println("Connected!");
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  connectToWifi();
  ThingSpeak.begin(client);
}

void loop() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      currentSoil = data.substring(0, commaIndex).toInt();
      currentPump = data.substring(commaIndex + 1).toInt();
    }
  }

  if (millis() - lastUploadTime > 16000) {
    if(WiFi.status() != WL_CONNECTED) connectToWifi();

    ThingSpeak.setField(1, currentSoil);
    ThingSpeak.setField(2, currentPump);
    
    int x = ThingSpeak.writeFields(CHANNEL_ID, API_KEY);
    
    if(x == 200){
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastUploadTime = millis();
  }
}