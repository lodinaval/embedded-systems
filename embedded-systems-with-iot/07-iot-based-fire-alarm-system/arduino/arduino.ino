#include <SoftwareSerial.h>

#define fireSensor1 4
#define fireSensor2 5
#define fireSensor3 6
#define fireSensor4 7
#define buzzerPin 8

// RX = Pin 2, TX = Pin 3
SoftwareSerial espSerial(2,3); 

unsigned long timeNow = 0;
const int sendInterval = 1000; // Send data every 1 second

int fire1 = 0;
int fire2 = 0;
int fire3 = 0;
int fire4 = 0;
int buzzerState = 0; 

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(fireSensor1, INPUT);
  pinMode(fireSensor2, INPUT);
  pinMode(fireSensor3, INPUT);
  pinMode(fireSensor4, INPUT);
}

void readSensors() {
  // Read each sensor independently (assuming active LOW)
  fire1 = !digitalRead(fireSensor1);
  fire2 = !digitalRead(fireSensor2);
  fire3 = !digitalRead(fireSensor3);
  fire4 = !digitalRead(fireSensor4);

  if(fire1 || fire2 || fire3 || fire4) {
    digitalWrite(buzzerPin, HIGH);
    buzzerState = 1;
  } else {
    digitalWrite(buzzerPin, LOW);
    buzzerState = 0;
  }
}

void loop() {
  readSensors();

  if(millis() - timeNow >= sendInterval) { 
    timeNow = millis();
    
    espSerial.print(fire1);
    espSerial.print(",");
    espSerial.print(fire2);
    espSerial.print(",");
    espSerial.print(fire3);
    espSerial.print(",");
    espSerial.print(fire4);
    espSerial.print(",");
    espSerial.println(buzzerState);
  }
}