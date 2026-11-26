#include <ThingSpeak.h>
#include <WiFi.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT_MS 15000
#define CHANNEL_ID 3338656
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
  if(Serial2.available()){
    String data = Serial2.readStringUntil('\n');
    int c1 = data.indexOf(',');
    int c2 = data.indexOf(',', c1 + 1);
    int c3 = data.indexOf(',', c2 + 1);
    int c4 = data.indexOf(',', c3 + 1);
    if(c1 > 0){
      int led = data.substring(0, c1).toInt();
      int buzzer = data.substring(c1 + 1, c2).toInt();
      int lcd = data.substring(c2 + 1, c3).toInt();
      int ssd = data.substring(c3 + 1, c4).toInt();
      int motor = data.substring(c4 + 1).toInt();
      ThingSpeak.setField(1, led);
      ThingSpeak.setField(2, buzzer);
      ThingSpeak.setField(3, lcd);
      ThingSpeak.setField(4, ssd);
      ThingSpeak.setField(5, motor);
      ThingSpeak.writeFields(CHANNEL_ID, API_KEY);
      Serial.print(led);
      Serial.print(buzzer);
      Serial.print(lcd);
      Serial.print(ssd);
      Serial.println(motor);
    }
  }
}
