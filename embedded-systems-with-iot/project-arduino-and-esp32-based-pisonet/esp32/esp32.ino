#include <ThingSpeak.h>
#include <WiFi.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT_MS 15000
#define CHANNEL_ID 3401245
#define API_KEY "YOUR_THINGSPEAK_WRITE_API_KEY"

WiFiClient client;

unsigned long lastUploadTime = 0;
// Set to 16 seconds. ThingSpeak strictly rejects anything faster than 15.0 seconds. 
// Using 16s prevents data from being rejected due to minor network delays.
const int postingInterval = 16000; 

void connectToWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    delay(100);
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  connectToWifi();
  ThingSpeak.begin(client);
}

void loop(){
  // Auto-reconnect if WiFi drops, preventing the system from freezing permanently
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }

  if(Serial2.available()){
    String data = Serial2.readStringUntil('\n');
    data.trim();
    
    // Find the 5 commas separating the 6 variables
    int comma1 = data.indexOf(',');
    int comma2 = data.indexOf(',', comma1 + 1);
    int comma3 = data.indexOf(',', comma2 + 1);
    int comma4 = data.indexOf(',', comma3 + 1);
    int comma5 = data.indexOf(',', comma4 + 1);

    if(comma1 > 0 && comma5 > 0){
      String timeStr = data.substring(0, comma1);
      String moneyStr = data.substring(comma1 + 1, comma2);
      String monitorStr = data.substring(comma2 + 1, comma3);
      String boxStr = data.substring(comma3 + 1, comma4);
      String tempStr = data.substring(comma4 + 1, comma5);
      String humStr = data.substring(comma5 + 1);

      if(millis() - lastUploadTime > postingInterval){
        
        long tSec = timeStr.toInt();
        int tMinGraph = tSec / 60; // Send minutes for clean graphing
        
        // Format text for Status Box (HH:MM:SS)
        int h = tSec / 3600;
        int m = (tSec % 3600) / 60;
        int s = tSec % 60;
        char timeFormatted[20];
        sprintf(timeFormatted, "Time: %02d:%02d:%02d", h, m, s);

        ThingSpeak.setField(1, tMinGraph);
        ThingSpeak.setField(2, moneyStr.toInt());
        ThingSpeak.setField(3, monitorStr.toInt());
        ThingSpeak.setField(4, boxStr.toInt());
        ThingSpeak.setField(5, tempStr.toFloat());
        ThingSpeak.setField(6, humStr.toFloat());
        
        // Push the text format to the Status Box
        ThingSpeak.setStatus(String(timeFormatted));
        
        ThingSpeak.writeFields(CHANNEL_ID, API_KEY);
        
        lastUploadTime = millis();
      }
    }
  }
}