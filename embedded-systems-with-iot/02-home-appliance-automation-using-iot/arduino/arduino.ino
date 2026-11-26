#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>
#include <IRremote.hpp>

#define CLK 9
#define DIO 8
#define IR_PIN 11
#define BUZZER_PIN 7
#define LED_PIN 6
#define MOTOR_PIN 5

//1 - F30CFF00 - LCD
//2 - E718FF00 - BUZZER
//3 - A15EFF00 - LED
//4 - F708FF00 - MOTOR
//5 - E31CFF00 - SSD
//+ - EA15FF00 - INCREMENT NUM ON SSD
//- - F807FF00 - DECREMENT NUM ON SSD

bool LED_ON = LOW;
bool BUZZER_ON = LOW;
bool LCD_ON = LOW;
bool SSD_ON = LOW;
bool MOTOR_ON = LOW;
//Statuses for the ESP32 to Read
int LED_STATUS = 0;
int BUZZER_STATUS = 0;
int LCD_STATUS = 0;
int SSD_STATUS = 0;
int MOTOR_STATUS = 0;
int pos = 0; //Angle of motor
int num = 0; //Number on SSD
TM1637Display ssd(CLK, DIO);
LiquidCrystal_I2C lcd(0x27,16,2);
Servo motor;

SoftwareSerial espSerial(2,3); //Rx, Tx
unsigned long lastSendTime = 0;

void setup() {
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
  espSerial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.init();
  lcd.noBacklight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  ssd.setBrightness(0x0f);
  motor.attach(MOTOR_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(IrReceiver.decode()){
    Serial.print(LED_ON);
    if(IrReceiver.decodedIRData.decodedRawData == 0xF30CFF00) LCD_ON=!LCD_ON;
    if(IrReceiver.decodedIRData.decodedRawData == 0xE718FF00) BUZZER_ON=!BUZZER_ON;
    if(IrReceiver.decodedIRData.decodedRawData == 0xA15EFF00) LED_ON=!LED_ON;
    if(IrReceiver.decodedIRData.decodedRawData == 0xF708FF00) MOTOR_ON=!MOTOR_ON;
    if(IrReceiver.decodedIRData.decodedRawData == 0xE31CFF00) SSD_ON=!SSD_ON;
    if(IrReceiver.decodedIRData.decodedRawData == 0xEA15FF00 && SSD_ON) num++;
    if(IrReceiver.decodedIRData.decodedRawData == 0xF807FF00 && SSD_ON) num--;
    IrReceiver.resume();
  }

  if(LCD_ON){
    lcd.backlight();
    LCD_STATUS = 1;
  }
  else {
    lcd.noBacklight();
    LCD_STATUS = 0;
  }
  if(LED_ON){
    digitalWrite(LED_PIN, HIGH);
    LED_STATUS = 1;
  }
  else {
    digitalWrite(LED_PIN, LOW);
    LED_STATUS = 0;
  }
  if(BUZZER_ON){
    digitalWrite(BUZZER_PIN, HIGH);
    BUZZER_STATUS = 1;
  }
  else{
    digitalWrite(BUZZER_PIN, LOW);
    BUZZER_STATUS = 0;
  }
  if(MOTOR_ON){
    motor.write(120);
    MOTOR_STATUS = 1;
  }
  else {
    motor.write(0);
    MOTOR_STATUS = 0;
  }
  if(SSD_ON){
    ssd.showNumberDec(num, true);
    SSD_STATUS = 1;
  }
  else{
    ssd.clear();
    SSD_STATUS = 0;
  }
  if (millis() - lastSendTime >= 15000) {
    espSerial.print(LED_STATUS);
    espSerial.print(",");
    espSerial.print(BUZZER_STATUS);
    espSerial.print(",");
    espSerial.print(LCD_STATUS);
    espSerial.print(",");
    espSerial.print(SSD_STATUS);
    espSerial.print(",");
    espSerial.println(MOTOR_STATUS);
    lastSendTime = millis(); // Reset the timer
  }
  Serial.print(LED_STATUS);
  Serial.print(",");
  Serial.print(BUZZER_STATUS);
  Serial.print(",");
  Serial.print(LCD_STATUS);
  Serial.print(",");
  Serial.print(SSD_STATUS);
  Serial.print(",");
  Serial.println(MOTOR_STATUS);
}
