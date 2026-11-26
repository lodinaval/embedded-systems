
#include <TM1637Display.h>

#define CLK 12
#define DIO 13
#define RED_PIN 2
#define YELLOW_PIN 3
#define GREEN_PIN 4
#define BUT1_PIN 6 //INCREMENT TIME
#define BUT2_PIN 7 // DECREMENT TIME
#define TRIG_PIN 8
#define ECHO_PIN 9
#define BUZZER_PIN 10
#define SECOND_DELAY 1000
#define MAX_DISTANCE 300

TM1637Display display(CLK, DIO);

int timer = 0;
int maxRed = 60;
int distance = 0; 
//STATE: 
//0 - GREEN
//1 - YELLOW
//2 - RED
int state = 2; //Initially at RED STATE
void setup() {
  Serial.begin(9600);
  pinMode(BUT1_PIN, INPUT_PULLUP);
  pinMode(BUT2_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  display.setBrightness(0x0a);
}

void printTime(){
  Serial.print("State: ");
  Serial.println(state);
  Serial.println(timer);
  display.showNumberDec(timer, false);
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}

void changeTime(){
  if(digitalRead(BUT1_PIN) == LOW && state == 2){
    if(timer < 30) maxRed = 45;
  }
  if(digitalRead(BUT2_PIN) == LOW && state == 0){
    timer+=30;
    if(timer > 60) timer = 60;
  }
}

void trafficLight(){
  while(state == 2){
    changeTime();
    printTime();
    if(getDistance() < 30){
      tone(BUZZER_PIN, 1000);
    }
    else{
      noTone(BUZZER_PIN);
    }
    digitalWrite(RED_PIN, HIGH);
    timer++;
    delay(SECOND_DELAY);
    if(timer > maxRed){
      digitalWrite(RED_PIN, LOW);
      state = 0;
      maxRed = 60;
      timer = 60;
    }
  }

  while(state == 0){
    changeTime();
    printTime();
    digitalWrite(GREEN_PIN, HIGH);
    timer--;
    delay(SECOND_DELAY);
    if(timer < 30){
      digitalWrite(GREEN_PIN, LOW);
      state = 1;
    }
  }

  while(state == 1){
    changeTime();
    printTime();
    digitalWrite(YELLOW_PIN, HIGH);
    timer--;
    delay(SECOND_DELAY);
    if(timer < 0){
      digitalWrite(YELLOW_PIN, LOW);
      state = 2;
      timer = 0;
    }
  }
}
void loop() {
  trafficLight();
}
