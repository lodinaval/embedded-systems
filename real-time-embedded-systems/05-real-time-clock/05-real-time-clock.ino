#include <Ds1302.h>
#include <Arduino.h>
#include <TM1637Display.h>
#include <SevSeg.h>

#define CLK 14
#define DIO 15
#define RTC_RST 17
#define RTC_DATA 18
#define RTC_CLK 19

SevSeg sevseg;
TM1637Display display(CLK, DIO);
unsigned long lastUpdate = 0;
Ds1302 rtc(RTC_RST, RTC_DATA, RTC_CLK);

void setup() {
  rtc.init();
  Serial.begin(9600);
  display.setBrightness(0x0f);
  byte numDigits = 4;   
  byte digitPins[] = {2, 3, 4, 5}; //Digits: 1,2,3,4
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; //Segments: A,B,C,D,E,F,G,Period

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(10);

}

void loop() {
  Ds1302::DateTime now;
  rtc.getDateTime(&now);
  static uint8_t last_second = 0;

  if (last_second != now.second)
    {
      last_second = now.second;
      Serial.print(now.hour);
      sevseg.setNumber(now.hour);
      display.showNumberDecEx(now.minute, 0b01000000, true, 2, 0);
      display.showNumberDecEx(now.second, 0b01000000, true, 2, 2);
    }
  sevseg.refreshDisplay();
}
