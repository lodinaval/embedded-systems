#include <Servo.h>
#include <DHT11.h>
#include <LiquidCrystal.h>


DHT11 dht(7);
Servo servo;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int temperature = 0;
int humidity = 0;
int angle = 10;

void setup() {
  Serial.begin(9600);

  //LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  //Servo
  servo.attach(8);
  servo.write(angle);
}

void servoActivate(){
  for(angle = 10; angle < 180; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 

  for(angle = 180; angle > 10; angle--)    
  {                                
    servo.write(angle);           
    delay(15);       
  } 
}

void loop() {

  dht.readTemperatureHumidity(temperature, humidity);

  //Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);

  // LCD
  lcd.clear();  // clear old text
  lcd.setCursor(0, 0); 
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1); 
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  if(temperature>=80 && humidity >= 90){
    servoActivate();
  }


  delay(2000); // DHT11 needs ~2s between reads
}