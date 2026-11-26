#include <ThingSpeak.h>
#include <WiFi.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_TIMEOUT_MS 15000
#define CHANNEL_ID 3386734
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
      

      String fire1 = data.substring(0, comma1);
      String fire2 = data.substring(comma1 + 1, comma2);
      String fire3 = data.substring(comma2 + 1, comma3);
      String fire4 = data.substring(comma3 + 1, comma4);
      String buzzer = data.substring(comma4 + 1);
      Serial.print(fire1);
      Serial.print(fire2);
      Serial.print(fire3);
      Serial.print(fire4);
      Serial.println(buzzer);


      if(millis() - lastUploadTime > postingInterval){
        ThingSpeak.setField(1, fire1.toInt());
        ThingSpeak.setField(2, fire2.toInt());
        ThingSpeak.setField(3, fire3.toInt());
        ThingSpeak.setField(4, fire4.toInt());
        ThingSpeak.setField(5, buzzer.toInt());
        
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