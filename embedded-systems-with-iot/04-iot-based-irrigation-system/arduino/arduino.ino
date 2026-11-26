#include <SoftwareSerial.h>

const int SENSOR_PIN = A1;
const int PUMP_PIN = 4;

int sensorValue = 0;
int pump_state = 0;
bool isWatering = false;
unsigned long timeNow = 0;
unsigned long pumpStartTime = 0;

SoftwareSerial espSerial(2, 3);

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
}

void loop() {
  if (millis() - timeNow > 500) {
    timeNow = millis();
    sensorValue = analogRead(SENSOR_PIN);

    String stateLabel = "";
    if (sensorValue <= 450) {
      stateLabel = "Super Wet";
    } else if (sensorValue <= 550) {
      stateLabel = "Wet";
    } else if (sensorValue <= 650) {
      stateLabel = "Wet But Not Dry";
    } else if (sensorValue <= 800) {
      stateLabel = "Normal";
    } else {
      stateLabel = "Super Dry";
    }

    Serial.print("Value: ");
    Serial.print(sensorValue);
    Serial.print(" | State: ");
    Serial.println(stateLabel);

    espSerial.print(sensorValue);
    espSerial.print(",");
    espSerial.println(pump_state);
  }

  if (sensorValue >= 1000 && !isWatering) {
    digitalWrite(PUMP_PIN, HIGH);
    pump_state = 1;
    pumpStartTime = millis();
    isWatering = true;
  }

  if (isWatering) {
    if (millis() - pumpStartTime > 2000) {
      digitalWrite(PUMP_PIN, LOW);
      pump_state = 0;
      isWatering = false;
    }
  }
}