#include <DHT11.h>
#include <SoftwareSerial.h>

#define DHT_PIN A0

DHT11 dht(DHT_PIN);
SoftwareSerial espSerial(2,3); //Rx,Tx
unsigned long lastSendTime = 0;

void printDHT(){
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();

  if (millis() - lastSendTime >= 20000) { 
    espSerial.print(temp);
    espSerial.print(",");
    espSerial.println(hum);
    Serial.print(temp);
    Serial.print(hum);
    lastSendTime = millis();
  }
}

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
}

void loop() {
  printDHT();
}
