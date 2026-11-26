#include <SoftwareSerial.h>
#include <NewPing.h>
#define sensor1trig A0
#define sensor1echo A1
#define sensor2trig A2
#define sensor2echo A3

int sensor1 = 0;
int sensor2 = 0;
unsigned long timeNow = 0;

int numPerson1 = 0;
int numPerson2 = 0;
int numPerson3 = 0;
int numPerson4 = 0;
int numPersonTotal = 0;

bool active1 = false; 
bool active2 = false; 

NewPing sonar1(sensor1trig, sensor1echo, 50);
NewPing sonar2(sensor2trig, sensor2echo, 50);
SoftwareSerial espSerial(A4,A5);

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  for(int i = 2; i < 14; i++){
    pinMode(i, OUTPUT);
  }
}

void loop() {
  readSensor();
}

void readSensor(){
  if(millis() - timeNow > 500){
    timeNow = millis();
    sensor1 = sonar1.ping_cm();
    sensor2 = sonar2.ping_cm();
    
    Serial.print("Sensor 1: ");
    Serial.println(sensor1);
    Serial.print("Sensor 2: ");
    Serial.println(sensor2);
    
    add();
  } 
}

void add(){
  if(sensor1 > 0 && sensor1 < 40){
    if(active1 == false){ 
      active1 = true;
      
      if(sensor1 < 10)      { if(numPerson1 < 3) numPerson1 += 1; light(1); }
      else if(sensor1 < 20) { if(numPerson2 < 3) numPerson2 += 1; light(2); }
      else if(sensor1 < 30) { if(numPerson3 < 3) numPerson3 += 1; light(3); }
      else if(sensor1 < 40) { if(numPerson4 < 3) numPerson4 += 1; light(4); }
    }
  } else {
    active1 = false; 
  }


  if(sensor2 > 0 && sensor2 < 40){
    if(active2 == false){ 
      active2 = true; // Lock it!
      
      if(sensor2 < 10)      { if(numPerson1 > 0) numPerson1 -= 1; light(1); }
      else if(sensor2 < 20) { if(numPerson2 > 0) numPerson2 -= 1; light(2); }
      else if(sensor2 < 30) { if(numPerson3 > 0) numPerson3 -= 1; light(3); }
      else if(sensor2 < 40) { if(numPerson4 > 0) numPerson4 -= 1; light(4); }
    }
  } else {
    active2 = false; 
  }
  numPersonTotal = numPerson1 + numPerson2 + numPerson3 + numPerson4;
  Serial.print("total: ");
  Serial.println(numPersonTotal);
  espSerial.print(numPerson1);
  espSerial.print(",");
  espSerial.print(numPerson2);
  espSerial.print(",");
  espSerial.print(numPerson3);
  espSerial.print(",");
  espSerial.print(numPerson4);
  espSerial.print(",");
  espSerial.println(numPersonTotal);
}

void light(int floor){
  if(floor == 1){
    if(numPerson1 == 0){ digitalWrite(13, LOW);  digitalWrite(12, LOW);  digitalWrite(11, LOW);  }
    if(numPerson1 == 1){ digitalWrite(13, HIGH); digitalWrite(12, LOW);  digitalWrite(11, LOW);  }
    if(numPerson1 == 2){ digitalWrite(13, HIGH); digitalWrite(12, HIGH); digitalWrite(11, LOW);  }
    if(numPerson1 == 3){ digitalWrite(13, HIGH); digitalWrite(12, HIGH); digitalWrite(11, HIGH); }
  }
  
  if(floor == 2){
    if(numPerson2 == 0){ digitalWrite(10, LOW);  digitalWrite(9, LOW);  digitalWrite(8, LOW);  }
    if(numPerson2 == 1){ digitalWrite(10, HIGH); digitalWrite(9, LOW);  digitalWrite(8, LOW);  }
    if(numPerson2 == 2){ digitalWrite(10, HIGH); digitalWrite(9, HIGH); digitalWrite(8, LOW);  }
    if(numPerson2 == 3){ digitalWrite(10, HIGH); digitalWrite(9, HIGH); digitalWrite(8, HIGH); }
  }
  
  if(floor == 3){
    if(numPerson3 == 0){ digitalWrite(7, LOW);  digitalWrite(6, LOW);  digitalWrite(5, LOW);  }
    if(numPerson3 == 1){ digitalWrite(7, HIGH); digitalWrite(6, LOW);  digitalWrite(5, LOW);  }
    if(numPerson3 == 2){ digitalWrite(7, HIGH); digitalWrite(6, HIGH); digitalWrite(5, LOW);  }
    if(numPerson3 == 3){ digitalWrite(7, HIGH); digitalWrite(6, HIGH); digitalWrite(5, HIGH); }
  }
  
  if(floor == 4){
    if(numPerson4 == 0){ digitalWrite(4, LOW);  digitalWrite(3, LOW);  digitalWrite(2, LOW);  }
    if(numPerson4 == 1){ digitalWrite(4, HIGH); digitalWrite(3, LOW);  digitalWrite(2, LOW);  }
    if(numPerson4 == 2){ digitalWrite(4, HIGH); digitalWrite(3, HIGH); digitalWrite(2, LOW);  }
    if(numPerson4 == 3){ digitalWrite(4, HIGH); digitalWrite(3, HIGH); digitalWrite(2, HIGH); }
  }
}