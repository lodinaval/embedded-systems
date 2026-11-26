#include <SoftwareSerial.h>
#define LED_PIN 12
#define LIGHT_PIN A0
SoftwareSerial espSerial(2,3); //Rx, Tx

int threshold = 10;
int led_status = 0;
unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LIGHT_PIN, INPUT);
}

void loop() {
  int light = analogRead(A0);
  if(light < threshold){
    digitalWrite(LED_PIN, HIGH);
    led_status = 1;
  }
  else{
    digitalWrite(LED_PIN, LOW);
    led_status = 0;
  }
  
  if (millis() - lastSendTime >= 15000) {
    espSerial.print(light);
    espSerial.print(",");
    espSerial.println(led_status);
    lastSendTime = millis(); // Reset the timer
  }
  Serial.print(light);
  Serial.print(",");
  Serial.println(led_status);
}
