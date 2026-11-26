#include <LCD-I2C.h>
#include <Wire.h>

int sensor = 0;
int pump_pin = 2;
LCD_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    lcd.begin(&Wire);
    lcd.display();
    lcd.backlight();
    pinMode(pump_pin, OUTPUT);
    digitalWrite(pump_pin, LOW);
}

void loop()
{
    // Super Dry = 417, Normal = 390, Wet But Not Dry = 370, Wet = 350, Super Wet = 330
    sensor = analogRead(A0);
    Serial.println(sensor);

    lcd.setCursor(0,0);
    lcd.print("Moisture: ");
    lcd.print(sensor);
    lcd.setCursor(0,1);
    if (sensor >= 417) lcd.print("Super Dry");
    else if (sensor >= 390) lcd.print("Normal");
    else if (sensor >= 370) lcd.print("Wet But Not Dry");
    else if (sensor >= 350) lcd.print("Wet");
    else lcd.print("Super Wet");

    if(sensor >= 390){
        digitalWrite(pump_pin, HIGH);
        delay(2000); //Waters plant for 2 seconds
        digitalWrite(pump_pin, LOW);
        delay(60000); //1 Minute Cooldown
    }
}
