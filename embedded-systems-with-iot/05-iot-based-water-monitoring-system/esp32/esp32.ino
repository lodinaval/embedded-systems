#include <ThingSpeak.h>
#include <WiFi.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT_MS 15000
#define CHANNEL_ID 3370914
#define API_KEY "YOUR_THINGSPEAK_WRITE_API_KEY"


WiFiClient client;

void connectToWifi(){
  Serial.println("Connecting to wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED) Serial.println("Connecting to WiFi Failed!");
  else{
    Serial.println("Successfully connected to ");
    Serial.print(WiFi.localIP());
  }
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
    float flowRate = data.toFloat();
    Serial.println(flowRate);
    ThingSpeak.setField(1, flowRate);
    int x = ThingSpeak.writeFields(CHANNEL_ID, API_KEY);
  }
}
