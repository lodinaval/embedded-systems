#include <NewPing.h>
#include <Stepper.h>
#include <Servo.h>
#define TRIGGER_PIN 15
#define ECHO_PIN 16
#define MAX_DISTANCE 200

int stepsPerRev = 2048;
int rpm = 10;
int distance = 0;
Stepper stepper(stepsPerRev, 8,10,9,11);
Servo servo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  servo.attach(14);
    //Open Door
  stepper.setSpeed(rpm);
}

void loop() {
  distance = sonar.ping_cm();
  delay(100);
  Serial.println(distance);
  if(distance < 5){
    Serial.println(distance);
    servo.write(0); //Close door
    stepper.step(128); //Spin crusher
  }
  else{
    servo.write(90);
  }
}