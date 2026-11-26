#include "SevSeg.h"
SevSeg sevseg;

const uint8_t btnPins[5] = {A0, A1, A2, A3, A4};
bool lastState[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long modeLabel = 0;
const unsigned long modeLabelMS = 1200;
unsigned long lastDebounceTime[5] = {0, 0, 0, 0, 0};
const unsigned long debounceMs = 30;
int A[3] = {0, 0, 0}; //Array to combine 4 button inputs
bool showingResult = false;
int modeCounter = 1;
bool showModeLabel = false;
int base = 0;

void showMin3RightAligned(int value) {
  // Always renders 7 segment with leading zeroes
  if (value < 1000) {
    int h = (value / 100) % 10;
    int t = (value / 10)  % 10;
    int o =  value        % 10;

    char buf[5] = {' ', char('0' + h), char('0' + t), char('0' + o), '\0'};
    sevseg.setChars(buf);
  } else {
    // For 1000–9999 just show normally
    sevseg.setNumber(value);
  }
}

bool buttonPressedEdge(int idx) {
  bool reading = digitalRead(btnPins[idx]);
  if (reading != lastState[idx]) {
    lastDebounceTime[idx] = millis();
    lastState[idx] = reading;
  }
  if ((millis() - lastDebounceTime[idx]) > debounceMs) {
    // detect press when it goes LOW (INPUT_PULLUP)
    if (reading == LOW) {
      // wait for release while keeping display alive
      while (digitalRead(btnPins[idx]) == LOW) {
        sevseg.refreshDisplay();
      }
      return true;
    }
  }
  return false;
}

void decimalToBinary(int num, char buffer[5]){
  char tmp[19];
  utoa(num, tmp, 2);
  buffer[0] = '0';
  buffer[1] = '0';
  buffer[2] = '0';
  buffer[3] = '0';
  buffer[4] = '0';

  if(strlen(tmp) <= 4){
    for(int i = 0; i < strlen(tmp); i++){
      buffer[4-strlen(tmp) + i] = tmp[i];
    }
  }
  else{
    buffer[0] = tmp[strlen(tmp) - 4];
    buffer[1] = tmp[strlen(tmp) - 3];
    buffer[2] = tmp[strlen(tmp) - 2];
    buffer[3] = tmp[strlen(tmp) - 1];
  }
}

void binaryToDecimal(int num){
  int sum = 0;
  int p = 1;            
  for (int i = 0; i < 3; i++) {
    int digit = num % 10;   
    num /= 10;              
    if (digit == 1) sum += p;
    p *= 2;                 
  }
  sevseg.setNumber(sum);
}

void octalToDecimal(int num){
  int sum = 0;
  int p = 1;            
  for (int i = 0; i < 3; i++) {
    int digit = num % 10;   
    num /= 10;              
    sum += p*digit;
    p *= 8;                 
  }
  sevseg.setNumber(sum);
}

int getInput(int base){
  for(int i = 0; i < 3; i++){
      if(buttonPressedEdge(i)){
        A[i] = (A[i] + 1) % base;
        showingResult = false;
      }
    }
    int num = 0;
    for(int x = 0; x < 3; x++){ 
      num = num*10 + A[x];
    }
    return num;
}

void setup(){
  Serial.begin(9600);
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
  for(int i = 0; i < 5; i++){
    pinMode(btnPins[i], INPUT_PULLUP);
  }
}

void loop(){
    getInput(base);
    if(buttonPressedEdge(4)){
      modeCounter++;
      if(modeCounter > 3) modeCounter = 1;
      showModeLabel = true;
      modeLabel = millis() + modeLabelMS;

      //resets inputs when switching modes.
      for (int i = 0; i < 3; i++) A[i] = 0;
      showingResult = false;
    }

    
    if(buttonPressedEdge(3)){
      if(modeCounter == 1){
        binaryToDecimal(getInput(base));
        showingResult = true;
      }

      if(modeCounter == 2){
        octalToDecimal(getInput(base));
        showingResult = true;
      }
      if(modeCounter == 3){
        char buffer[5];
        decimalToBinary(getInput(base), buffer);
        sevseg.setChars(buffer);
        showingResult = true;
      }
    }

    bool showingModeNow = showModeLabel && (long)(modeLabel - millis()) > 0;

    if (showingModeNow){
      if(modeCounter == 1){
        char display[4] = "BD";
        sevseg.setChars(display);
        sevseg.refreshDisplay();
        base = 2;
      }
      if(modeCounter == 2){
        char display[4] = "OD";
        sevseg.setChars(display);
        sevseg.refreshDisplay();
        base = 8;
      }
      if(modeCounter == 3){
        char display[4] = "DB";
        sevseg.setChars(display);
        sevseg.refreshDisplay();
        base = 10;
      }
    }
    else if(!showingResult){
      showMin3RightAligned(getInput(base));
    }
  sevseg.refreshDisplay();
}