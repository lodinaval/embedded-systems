#include <Servo.h>
#include <Key.h>
#include <Keypad.h>
#include <TM1637Display.h>
#include <RtcDS1302.h>

#define BUZZER_PIN A1
#define RELAY_PIN A2 
#define COIN_PIN 12
#define CLK 10
#define DIO 11
#define RTC_RST 17
#define RTC_DATA 18
#define RTC_CLK 19
#define SERVO_PIN A0 
#define password_length 6

TM1637Display ssd(CLK, DIO);
ThreeWire myWire(RTC_DATA, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> Rtc(myWire);
Servo servo;

//TIMER VARS
int totalSeconds = 0;
unsigned long previousTime = 0;
bool needsUpdate = true;

//COIN VARS
bool lastCoinState = LOW;
unsigned long lastCoinTime = 0;

//LOCKER VARS
char data[password_length+1]; 
char master[password_length+1] = "111111"; 
int data_count = 0; 

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void locker(){
  char key = customKeypad.getKey();
  if(key){
    if(key == 'D') { 
      data[data_count] = '\0'; 
      if(strcmp(data, master) == 0){
        Serial.println("\n[KEYPAD] Password Correct! Door Unlocked.");
        servo.write(90);
      } else {
        Serial.println("\n[KEYPAD] Wrong Password!");
      }
      data_count = 0; 
    } 
    else if (key == 'B') { 
      Serial.println("\n[KEYPAD] Door Locked!");
      servo.write(0);
      data_count = 0; 
    } 
    else if (key != 'A' && key != 'C' && key != '*' && key != '#') {
      if(data_count < password_length){
        data[data_count] = key;
        data_count++;
        Serial.print("*"); 
      }
    }
  }
}

void timer(){
  unsigned long currentTime = millis();
  
  if((currentTime - previousTime) >= 1000){
    previousTime = currentTime; 
    if(totalSeconds > 0){
      totalSeconds -= 1;
      needsUpdate = true;
    }
  }

  if(needsUpdate == true){
    if(totalSeconds > 0) digitalWrite(RELAY_PIN, LOW); 
    else digitalWrite(RELAY_PIN, HIGH); 

    int hour = totalSeconds / 3600;
    int minute = (totalSeconds % 3600) / 60;
    int second = totalSeconds % 60;

    if(totalSeconds <= 10 && totalSeconds > 0) digitalWrite(BUZZER_PIN, HIGH);
    else digitalWrite(BUZZER_PIN, LOW);
    
    Serial.print("Timer: ");
    Serial.print(hour);
    Serial.print(":");
    if(minute < 10) Serial.print("0");
    Serial.print(minute);
    Serial.print(":");
    if(second < 10) Serial.print("0");
    Serial.print(second);
    if(totalSeconds > 0) Serial.println(" (Monitor ON)");
    else Serial.println(" (Monitor OFF)");

    if(totalSeconds > 3600){
      ssd.showNumberDecEx(hour, 0b01000000, true, 2, 0);
      ssd.showNumberDecEx(minute, 0b01000000, true, 2, 2);
    }
    if(totalSeconds <= 3600 && totalSeconds > 0){
      ssd.showNumberDecEx(minute, 0b01000000, true, 2, 0);
      ssd.showNumberDecEx(second, 0b01000000, true, 2, 2);
    }
    
    needsUpdate = false;
  }
}

void displayRTC(){
  static uint8_t last_second = 0;
  RtcDateTime now = Rtc.GetDateTime();
  
  if(totalSeconds == 0){
    if (now.IsValid() && last_second != now.Second())
    {
      last_second = now.Second();
      
      Serial.print("Live Clock: ");
      Serial.print(now.Hour());
      Serial.print(":");
      if(now.Minute() < 10) Serial.print("0");
      Serial.println(now.Minute());

      ssd.showNumberDecEx(now.Hour(), 0b01000000, true, 2, 0);
      ssd.showNumberDecEx(now.Minute(), 0b01000000, true, 2, 2);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(COIN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  ssd.setBrightness(0x0f);
  
  Rtc.Begin();
  // Format: Year, Month, Day, Hour (24hr), Minute, Second
  //RtcDateTime manualTime = RtcDateTime(2026, 3, 9, 10, 10, 20); 
  
  //d Rtc.SetDateTime(manualTime);
  if (Rtc.GetIsWriteProtected()) {
      Rtc.SetIsWriteProtected(false);
  }
  if (!Rtc.GetIsRunning()) {
      Serial.println("RTC was stopped. Starting now...");
      Rtc.SetIsRunning(true);
  }
  
  servo.attach(SERVO_PIN); 
  servo.write(90);   
  digitalWrite(RELAY_PIN, HIGH); 
}

void loop() {
  bool currentCoinState = digitalRead(COIN_PIN);
  if (currentCoinState == HIGH && lastCoinState == LOW) {
    if (millis() - lastCoinTime > 200) {
      totalSeconds += 300;
      Serial.println("\n>>> COIN DROPPED: +5 Minutes <<<");
      needsUpdate = true;
      lastCoinTime = millis();
    }
  }
  lastCoinState = currentCoinState;

  locker(); 
  timer();
  displayRTC();
}