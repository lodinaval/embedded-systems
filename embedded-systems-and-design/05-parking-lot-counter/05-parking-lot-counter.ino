#include <NewPing.h>
#include <Servo.h>
#include "SevSeg.h"
#define TRIGGER_PIN 15
#define ECHO_PIN 16
#define MAX_DISTANCE 200

SevSeg sevseg;
Servo servo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int parkingSlots = 20;
const int maxSlots = 20;
bool sensor = false, gateDone = false, exitGate = false;
int distance = 0;

void distanceSensor(){
  distance = sonar.ping_cm();
  bool near = (distance > 0 && distance < 10);
  bool exitBand = (distance > 15 && distance < 40);
  sensor = near || exitBand;
  exitGate = exitBand;
}

void displayNum() {
  char buf[5];
  buf[0] = '0' + (parkingSlots / 10);
  buf[1] = '0' + (parkingSlots % 10);
  buf[2] = buf[0];
  buf[3] = buf[1];
  buf[4] = '\0';
  sevseg.setChars(buf);
}

void waitAndRefresh(unsigned int ms) {
  for (unsigned int i = 0; i < ms; i++) { sevseg.refreshDisplay(); delayMicroseconds(1000); }
}

void gateEnter(bool s){
  if(s) servo.write(90);
  waitAndRefresh(400);
  servo.write(0);
}

void setup() {
  Serial.begin(9600);
  byte numDigits = 4;  
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = 1;
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(60);
  servo.attach(14);
  servo.write(0);
}

void loop() {
  distanceSensor();
  Serial.println(distance);

  if(sensor){
    if(!gateDone){
      if(exitGate){
        if(parkingSlots < maxSlots) parkingSlots++;
      } else {
        if(parkingSlots > 0) parkingSlots--;
      }
      gateDone = true;
    }
  } else {
    gateDone = false;
    exitGate = false;
  }

  gateEnter(sensor && (exitGate || parkingSlots > 0));
  displayNum();
  sevseg.refreshDisplay();
}
