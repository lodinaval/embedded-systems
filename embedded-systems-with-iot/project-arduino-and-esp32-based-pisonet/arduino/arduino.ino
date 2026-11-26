#include <TM1637Display.h>
#include <SoftwareSerial.h>
#include <IRremote.hpp>
#include <DHT.h>

#define BUZZER_PIN 4
#define RELAY_PIN A2 
#define LDR_PIN A0     
#define LED_PIN 3      
#define COIN_PIN 2     // Hardware Interrupt Pin
#define IR_PIN 6 
#define DHTPIN 7       
#define DHTTYPE DHT11  
#define CLK 9
#define DIO 8
#define ESP_RX A3
#define ESP_TX A4

TM1637Display ssd(CLK, DIO);
SoftwareSerial espSerial(ESP_RX, ESP_TX);
DHT dht(DHTPIN, DHTTYPE);

volatile int pulseCount = 0;
volatile unsigned long lastPulseTime = 0;

long totalSeconds = 0;
unsigned long previousTime = 0;
bool needsUpdate = true;

long totalMonetary = 0; 
const int coinValue = 1;         
const int secondsPerPulse = 300; 

int lastBeepSecond = -1;
bool isBeeping = false;
unsigned long beepStartTime = 0;

bool adminOverride = false;
const int lightThreshold = 100; 

// Temp & Hum tracking
float currentTemp = 0.0;
float currentHum = 0.0;
unsigned long lastSensorRead = 0;

// --- HARDWARE INTERRUPT ---
void coinInterrupt() {
  unsigned long currentTime = millis();
  if (currentTime - lastPulseTime > 30) { 
    pulseCount++;
    lastPulseTime = currentTime;
  }
}

// --- RELAY KICKBACK FIX (HARDWARE FLAG CLEAR) ---
void setRelaySafe(int state) {
  int targetState = (state == 1) ? LOW : HIGH; // 1 = ON (LOW), 0 = OFF (HIGH)

  if (digitalRead(RELAY_PIN) != targetState) {
    detachInterrupt(digitalPinToInterrupt(COIN_PIN));
    
    digitalWrite(RELAY_PIN, targetState);
    delay(100); // Give EMI 100ms to completely dissipate
    
    // Clear the pending interrupt flag for INT0 (Pin 2)
    // Writing a 1 to this register clears the flag in AVR chips
    EIFR = bit(INTF0); 
    
    pulseCount = 0; 
    attachInterrupt(digitalPinToInterrupt(COIN_PIN), coinInterrupt, FALLING);
  }
}

void handleBuzzer() {
  if (currentTemp > 37.0) {
    if ((millis() / 200) % 2 == 0) {
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
    return; 
  }

  if (totalSeconds <= 60 && totalSeconds >= 0 && (totalSeconds % 10 == 0)) {
    if (totalSeconds != lastBeepSecond) {
      digitalWrite(BUZZER_PIN, HIGH);
      isBeeping = true;
      beepStartTime = millis();
      lastBeepSecond = totalSeconds;
    }
  }
  
  if (isBeeping && (millis() - beepStartTime >= 300)) {
    digitalWrite(BUZZER_PIN, LOW);
    isBeeping = false;
  } else if (!isBeeping) {
    digitalWrite(BUZZER_PIN, LOW); 
  }
}

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  dht.begin();
  
  pinMode(COIN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(COIN_PIN), coinInterrupt, FALLING);
  
  ssd.setBrightness(0x0f);
  ssd.showNumberDecEx(0, 0b01000000, true, 4, 0); 
  
  digitalWrite(RELAY_PIN, HIGH); 
  
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("System Initialized. Awaiting coins...");
}

void loop() {
  // --- PROCESS COIN PULSES ---
  if (pulseCount > 0) {
    noInterrupts(); 
    int pulsesToProcess = pulseCount;
    pulseCount = 0; 
    interrupts();   

    Serial.print(">>> PROCESSED ");
    Serial.print(pulsesToProcess);
    Serial.println(" PULSE(S) <<<");

    totalSeconds += (pulsesToProcess * secondsPerPulse);
    totalMonetary += (pulsesToProcess * coinValue);
    needsUpdate = true;
    lastBeepSecond = -1; 
  }

  // --- READ SENSORS (Non-Blocking) ---
  if (millis() - lastSensorRead >= 2000) {
    lastSensorRead = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      currentTemp = t;
      currentHum = h;
    }
  }

  // --- PHOTORESISTOR TAMPER ---
  int lightLevel = analogRead(LDR_PIN);
  int boxStatus = 0;
  if (lightLevel > lightThreshold) {
    digitalWrite(LED_PIN, HIGH); 
    boxStatus = 1;
  } else {
    digitalWrite(LED_PIN, LOW);  
    boxStatus = 0;
  }

  // --- IR ADMIN OVERRIDE ---
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.decodedRawData == 0xF30CFF00) {
      adminOverride = !adminOverride;
      needsUpdate = true;
      Serial.print("Admin Override: ");
      Serial.println(adminOverride ? "ON" : "OFF");
    }
    IrReceiver.resume();
  }

  // --- TIMER COUNTDOWN & STATUS UPDATE ---
  if (millis() - previousTime >= 1000) {
    previousTime = millis();
    if (totalSeconds > 0) {
      totalSeconds--;
    }
    needsUpdate = true; // Force 1-sec update for ESP32
  }

  // --- DISPLAY AND ESP SERIAL ---
  if (needsUpdate) {
    int monitorStatus = (totalSeconds > 0 || adminOverride) ? 1 : 0;
    
    // Call safe relay function to prevent ghost pulses
    setRelaySafe(monitorStatus);

    int hour = totalSeconds / 3600;
    int minute = (totalSeconds % 3600) / 60;
    int second = totalSeconds % 60;

    if (totalSeconds > 3600) {
      ssd.showNumberDecEx(hour, 0b01000000, true, 2, 0);
      ssd.showNumberDecEx(minute, 0b01000000, true, 2, 2);
    } else {
      ssd.showNumberDecEx(minute, 0b01000000, true, 2, 0);
      ssd.showNumberDecEx(second, 0b01000000, true, 2, 2);
    }
    
    Serial.print("[DEBUG] Timer: ");
    if (hour < 10) Serial.print("0");
    Serial.print(hour);
    Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.print(minute);
    Serial.print(":");
    if (second < 10) Serial.print("0");
    Serial.print(second);
    Serial.print(" | PHP: ");
    Serial.print(totalMonetary);
    Serial.print(" | Temp: ");
    Serial.print(currentTemp);
    Serial.print("C | Hum: ");
    Serial.print(currentHum);
    Serial.print("% | LDR: ");
    Serial.println(lightLevel);

    // Payload: Seconds, Money, Monitor, Box, Temp, Hum
    espSerial.print(totalSeconds);
    espSerial.print(",");
    espSerial.print(totalMonetary);
    espSerial.print(",");
    espSerial.print(monitorStatus); 
    espSerial.print(",");
    espSerial.print(boxStatus);
    espSerial.print(",");
    espSerial.print(currentTemp);
    espSerial.print(",");
    espSerial.println(currentHum);

    needsUpdate = false;
  }

  handleBuzzer();
}