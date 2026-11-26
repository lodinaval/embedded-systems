#include "SevSeg.h"
SevSeg sevseg;

int minute = 0;
int hour = 0;
int lastState0 = HIGH;
int lastState1 = HIGH;
int lastState2 = HIGH;

void waitAndRefresh(unsigned int ms) {
  for (unsigned int i = 0; i < ms; i++) {
    sevseg.refreshDisplay();   // keep multiplexing alive
    delayMicroseconds(1000);   // ~1 ms
  }
}

void displayTime(int hour, int minute) {
  char buf[5];
  buf[0] = '0' + (hour / 10);     // tens of hours
  buf[1] = '0' + (hour % 10);     // ones of hours
  buf[2] = '0' + (minute / 10);   // tens of minutes
  buf[3] = '0' + (minute % 10);   // ones of minutes
  buf[4] = '\0';

  sevseg.setChars(buf);  
}

void clock(){
  if(minute == 60){
    minute = 0;
    hour++;
    if(hour == 24){
      hour = 0;
    }
  }
  //Print for debugging
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);
  displayTime(hour, minute);
  minute++;
  waitAndRefresh(1000 * 60);   // wait ~1 minute
}

void getInput(){

  int s0 = digitalRead(A0);
  int s1 = digitalRead(A1);
  int s2 = digitalRead(A2);
  
  if (lastState0 == HIGH && s0 == LOW) {
    displayTime(hour,minute);
    Serial.print(digitalRead(A0));
    waitAndRefresh(30);
    hour = (hour+1) % 24;
  }
  
  if (lastState1 == HIGH && s1 == LOW) {
    displayTime(hour,minute);
    Serial.print(digitalRead(A1));
    waitAndRefresh(30);
    minute = (minute+1) % 60;
  }
  if (lastState2 == HIGH && s2 == LOW) {
    sevseg.blank();
    delay(1000);
    displayTime(hour,minute);
    clock();
  }

  lastState0 = s0;
  lastState1 = s1;
  lastState2 = s2;

}

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  
  byte numDigits = 4;  
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = 1;
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(60);
}

void loop() {
  getInput();
  displayTime(hour, minute);
  sevseg.refreshDisplay();
}
