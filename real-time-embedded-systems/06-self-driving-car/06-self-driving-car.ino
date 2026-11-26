#include <IRremote.hpp>
#define IR 8
int in1=3;
int in2=5;
int in3=6;
int in4=9;
int timeoutDelay = 700;
unsigned long lastSignalTime = 0;

// FWD - E718FF00
// BWD - AD52FF00
// LEFT - F708FF00
// RIGHT - A55AFF00

void Forward(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void Backward(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void Left(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void Right(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void Stop(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void setup() {
  // put your setup code here, to run once:
  IrReceiver.begin(IR, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.decodedRawData != 0) {
      lastSignalTime = millis();
      uint32_t command = IrReceiver.decodedIRData.decodedRawData;
      if (command == 0xE718FF00) Forward();
      else if (command == 0xAD52FF00) Backward();
      else if (command == 0xF708FF00) Left();
      else if (command == 0xA55AFF00) Right();
      
      Serial.println(command, HEX);
    }
    IrReceiver.resume(); // Receive the next value
  }

  // Check if enough time has passed since the last signal
  if (millis() - lastSignalTime > timeoutDelay) {
    Stop();
  }
}
