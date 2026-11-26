#include <LiquidCrystal_I2C.h>
#include <DHT22.h>

#define DHT_PIN A0

DHT22 dht22(DHT_PIN);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void printDHT(){
  float temp = dht22.getTemperature();
  float hum = dht22.getHumidity();
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humidity: ");
  Serial.println(hum);

  lcd.setCursor(1,0);
  lcd.print("Temp:      C");
  lcd.setCursor(7,0);
  lcd.print(temp);
  lcd.setCursor(2,1);
  lcd.print("Hum:      %");
  lcd.setCursor(7,1);
  lcd.print(hum);
  delay(100);
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  printDHT();
}
