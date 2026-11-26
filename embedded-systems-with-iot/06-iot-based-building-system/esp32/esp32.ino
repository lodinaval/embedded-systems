#include <ThingSpeak.h>
#include <WiFi.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT_MS 15000
#define CHANNEL_ID 3381055
#define API_KEY "YOUR_THINGSPEAK_WRITE_API_KEY"

WiFiClient client;

unsigned long lastUploadTime = 0; 
const int postingInterval = 20000; // 20 seconds

void connectToWifi(){
  Serial.println("Connecting to wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("\nConnecting to WiFi Failed!");
  } else {
    Serial.println("\nSuccessfully connected to ");
    Serial.print(WiFi.localIP());
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  connectToWifi();
  ThingSpeak.begin(client);
}
void loop(){
  if(Serial2.available()){
    String data = Serial2.readStringUntil('\n');
    data.trim();
    
    // 2. Find the positions of the 4 commas
    int comma1 = data.indexOf(',');
    int comma2 = data.indexOf(',', comma1 + 1);
    int comma3 = data.indexOf(',', comma2 + 1);
    int comma4 = data.indexOf(',', comma3 + 1);


    if(comma1 > 0 && comma2 > 0 && comma3 > 0 && comma4 > 0){
      

      String flr1 = data.substring(0, comma1);
      String flr2 = data.substring(comma1 + 1, comma2);
      String flr3 = data.substring(comma2 + 1, comma3);
      String flr4 = data.substring(comma3 + 1, comma4);
      String total = data.substring(comma4 + 1);


      Serial.println("Received: F1:" + flr1 + " F2:" + flr2 + " F3:" + flr3 + " F4:" + flr4 + " T:" + total);


      if(millis() - lastUploadTime > postingInterval){
        ThingSpeak.setField(1, flr1.toInt());
        ThingSpeak.setField(2, flr2.toInt());
        ThingSpeak.setField(3, flr3.toInt());
        ThingSpeak.setField(4, flr4.toInt());
        ThingSpeak.setField(5, total.toInt());
        
        int httpCode = ThingSpeak.writeFields(CHANNEL_ID, API_KEY);
        
        if(httpCode == 200){
          Serial.println("Channel update successful.");
        } else {
          Serial.println("Problem updating channel. HTTP error code " + String(httpCode));
        }
        
        lastUploadTime = millis();
      }
    }
  }
}