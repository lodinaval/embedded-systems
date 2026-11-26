#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 12
#define DIO 13
#define BUT1_PIN 8
#define BUT2_PIN 7
#define BUT3_PIN 6
#define BUT4_PIN 5
#define BUT5_PIN 4
#define TEST_DELAY 80
TM1637Display display(CLK, DIO);

//BUTTON 1 - TEAM A++
//BUTTON 2 - TEAM A--
//BUTTON 3 - TEAM B++
//BUTTON 4 - TEAM B--
//BUTTON 5 - RESET

int teamA = 0;
int teamB = 0;
int finalScore = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUT1_PIN, INPUT_PULLUP);
  pinMode(BUT2_PIN, INPUT_PULLUP);
  pinMode(BUT3_PIN, INPUT_PULLUP);
  pinMode(BUT4_PIN, INPUT_PULLUP);
  pinMode(BUT5_PIN, INPUT_PULLUP);
  display.clear();
}

void loop() {
  display.setBrightness(0x0a);

  if(digitalRead(BUT1_PIN)==LOW) teamA++;
  if(digitalRead(BUT2_PIN)==LOW) teamA--;
  if(digitalRead(BUT3_PIN)==LOW) teamB++;
  if(digitalRead(BUT4_PIN)==LOW) teamB--;
  if(digitalRead(BUT5_PIN)==LOW){
    teamA = 0;
    teamB = 0;
  }
  if(teamA < 0) teamA = 0;
  if(teamB < 0) teamB = 0;
  if(teamA > 99) teamA = 99;
  if(teamB > 99) teamB = 99;

  finalScore = (teamA*100) + teamB;
  display.showNumberDec(finalScore, true);
  delay(TEST_DELAY);

}
