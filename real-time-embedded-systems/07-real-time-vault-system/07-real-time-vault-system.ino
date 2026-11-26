#include <Key.h>
#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define password_length 6 
#define open_duration 10

char data[password_length+1]; 
char master[password_length+1] = "111111"; //+1 cause of \0 terminator

int data_count = 0; 
const byte ROWS = 4;
const byte COLS = 4;   
bool opened = LOW;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27,16,2);
Servo servo;

char customKey = customKeypad.getKey();
void checkPass(){

  lcd.clear();
  
  if(!strcmp(data, master)){
    lcd.print("Opening...");
    servo.write(90);
    lcd.clear();
    lcd.print("Opened!");
    opened = HIGH;
    timer(open_duration);
  }
  
  
  else{
    lcd.print("Wrong password!");
    delay(3000);
    lcd.clear();
    lcd.print("Password: ");
  }
  data_count = 0;
  lcd.setCursor(data_count, 1);
}

void lockDoor(){
  if(opened){
    lcd.clear();
    servo.write(0);
    lcd.print("Locked!");
    delay(2000);
    lcd.clear();
    lcd.print("Password: ");
    opened = LOW;
  }
}

void typeKey(char key){
  if(!opened && data_count < password_length) {
    if(key != 'A' && key != 'B' && key != 'C' && key != 'D'){
      data[data_count] = key;
      lcd.setCursor(data_count, 1);
      lcd.print(data[data_count]);
      data_count++;
    }
  }
}

void timer(int timer){
  unsigned long timeNow = millis();
  while(timer != 0){
    char key = customKeypad.getKey();
    if(key == 'B') timer+=5;
    lcd.setCursor(0,0);
    lcd.print("Opened!");
    lcd.setCursor(0,1);
    lcd.print(timer);
    if((millis() - timeNow) > 1000){
      timer--;
      timeNow = millis();
      lcd.clear();
    }
  }
  lockDoor();
}

void locker(){
  char customKey = customKeypad.getKey();
  if(customKey){
    if(customKey == 'D') checkPass();
    else (typeKey(customKey));
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("Password: ");
  servo.attach(10);
  servo.write(0);
}

void loop() {
  locker();
}