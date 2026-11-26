#include <SoftwareSerial.h>
#define waterSensor A0

SoftwareSerial espSerial(2,3);
unsigned long timeNow = 0;
int waterLevel = 0;
float flowRate = 0;
void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  pinMode(waterSensor, INPUT);
}

void loop() {
  if(millis() - timeNow > 15000){
    timeNow = millis();
    waterLevel = analogRead(waterSensor);
    flowRate = waterLevel / 50.265;
    Serial.print("Water Level: ");
    Serial.println(waterLevel);

    if(waterLevel >= 650) Serial.println("WATER IS FULL");
    else if(waterLevel >= 550 && waterLevel < 650) Serial.println("WATER IS 3/4");
    else if(waterLevel >= 520 && waterLevel < 550) Serial.println("WATER 1/2");
    else if(waterLevel >= 460 && waterLevel < 520) Serial.println("WATER 1/4");
    else if(waterLevel >= 300 && waterLevel < 460) Serial.println("WATER IS 1/8");
    else Serial.println("NO WATER");

    Serial.print("Flow Rate (mL/s): ");
    Serial.println(flowRate);
    espSerial.println(flowRate);
  }
}
