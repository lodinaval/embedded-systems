int LED1 = 2;
int LED2 = 3;
int LED3 = 4;
int LED4 = 5;
int LED5 = 6;
int LED6 = 7;
int LED7 = 8;
int LED8 = 9;
int BUT1 = 10;
int BUT2 = 11;
int BUT3 = 12;
int BUT4 = 13;
int BUT5 = A5;
const int ledCount = 8;
int leds[] = {2, 3, 4, 5, 6, 7, 8, 9};

int minVal = 1;
int maxVal = 100;
int randomInRange = (rand() % (maxVal - minVal + 1)) + minVal;

void clear(){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);
}

void patternAlternate(){
  digitalWrite(LED1, HIGH);
  delay(100);
  clear();
  digitalWrite(LED2, HIGH);
  delay(100);
  clear();
  digitalWrite(LED3, HIGH);
  delay(100);
  clear();
  digitalWrite(LED4, HIGH);
  delay(100);
  clear();
  digitalWrite(LED5, HIGH);
  delay(100);
  clear();
  digitalWrite(LED6, HIGH);
  delay(100);
  clear();
  digitalWrite(LED7, HIGH);
  delay(100);
  clear();
  digitalWrite(LED8, HIGH);
  delay(100);
  clear();
}

void randomLed(int count, int delayTime){
  clear();
  int chosen[count];
  for(int i = 0; i < count; i++){
    int r;
    bool duplicate;
    do{
      r = random(0, ledCount);
      duplicate = false;
      for(int j = 0; j < i; j++){
        if(chosen[j] == r){
          duplicate = true;
          break;
        }
      }
    }
    while(duplicate);
    chosen[i] = r;
    digitalWrite(leds[r], HIGH);
  }
  delay(delayTime);
}

void patternAlternate2(){
  digitalWrite(LED8, HIGH);
  delay(100);
  clear();
  digitalWrite(LED7, HIGH);
  delay(100);
  clear();
  digitalWrite(LED6, HIGH);
  delay(100);
  clear();
  digitalWrite(LED5, HIGH);
  delay(100);
  clear();
  digitalWrite(LED4, HIGH);
  delay(100);
  clear();
  digitalWrite(LED3, HIGH);
  delay(100);
  clear();
  digitalWrite(LED2, HIGH);
  delay(100);
  clear();
  digitalWrite(LED1, HIGH);
  delay(100);
  clear();
}

void patternAlternate3(){
  digitalWrite(LED1, HIGH);
  delay(20);
  clear();
  digitalWrite(LED2, HIGH);
  delay(20);
  clear();
  digitalWrite(LED3, HIGH);
  delay(20);
  clear();
  digitalWrite(LED4, HIGH);
  delay(20);
  clear();
  digitalWrite(LED5, HIGH);
  delay(20);
  clear();
  digitalWrite(LED6, HIGH);
  delay(20);
  clear();
  digitalWrite(LED7, HIGH);
  delay(20);
  clear();
  digitalWrite(LED8, HIGH);
  delay(20);
  clear();
}

void patternAlternate4(){
  digitalWrite(LED8, HIGH);
  delay(20);
  clear();
  digitalWrite(LED7, HIGH);
  delay(20);
  clear();
  digitalWrite(LED6, HIGH);
  delay(20);
  clear();
  digitalWrite(LED5, HIGH);
  delay(20);
  clear();
  digitalWrite(LED4, HIGH);
  delay(20);
  clear();
  digitalWrite(LED3, HIGH);
  delay(20);
  clear();
  digitalWrite(LED2, HIGH);
  delay(20);
  clear();
  digitalWrite(LED1, HIGH);
  delay(20);
  clear();
}

void sweetBeat(){
  digitalWrite(LED4, HIGH);
  digitalWrite(LED7, HIGH);
  delay(250);
  clear();
  digitalWrite(LED2, HIGH);
  digitalWrite(LED5, HIGH);
  delay(250);
  clear();
  digitalWrite(LED4, HIGH);
  digitalWrite(LED7, HIGH);
  delay(250);
  clear();
  digitalWrite(LED2, HIGH);
  digitalWrite(LED5, HIGH);
  delay(250);
  clear();
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
  delay(400);
  clear();
  digitalWrite(LED1, HIGH);
  digitalWrite(LED8, HIGH);
  delay(450);
  clear();
}

void allOn(){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
  digitalWrite(LED6, HIGH);
  digitalWrite(LED7, HIGH);
  digitalWrite(LED8, HIGH);
}

void paranoidBeat(){
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED5, HIGH);
    digitalWrite(LED8, HIGH);
    delay(300);
    clear();
    digitalWrite(LED1, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED6, HIGH);
    digitalWrite(LED7, HIGH);
    delay(300);
    clear();
    digitalWrite(LED1, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED5, HIGH);
    digitalWrite(LED7, HIGH);
    delay(300);
    clear();
    digitalWrite(LED2, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED6, HIGH);
    digitalWrite(LED8, HIGH);
    delay(300);
    clear();
}

void coldIntroBeat(){
  delay(300);
    digitalWrite(LED8, HIGH);
    delay(300);
    digitalWrite(LED6, HIGH);
    delay(300);
    digitalWrite(LED7, HIGH);
    delay(300);
    digitalWrite(LED5, HIGH);
    delay(300);
    digitalWrite(LED4, HIGH);
    delay(300);
    digitalWrite(LED2, HIGH);
    delay(300);
    digitalWrite(LED3, HIGH);
    delay(300);
    digitalWrite(LED1, HIGH);
    delay(300);
    clear();
    delay(300);
    allOn();
    delay(400);
    clear();
    delay(850);
    digitalWrite(LED2, HIGH);
    delay(100);
    clear();
    digitalWrite(LED1, HIGH);
    delay(100);
    clear();
    digitalWrite(LED8, HIGH);
    delay(500);
    clear();
    delay(300);
}

void coldBeat(){
  digitalWrite(LED8, HIGH);
  digitalWrite(LED7, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED5, HIGH);
  delay(650);
  digitalWrite(LED8, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED5, LOW);
  delay(650);
  digitalWrite(LED8, HIGH);
  digitalWrite(LED7, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED4, HIGH);
  delay(650);
  digitalWrite(LED8, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED4, LOW);
  delay(650);
  digitalWrite(LED8, HIGH);
  digitalWrite(LED7, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED6, HIGH);
  delay(650);
  digitalWrite(LED8, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED6, LOW);
  delay(650);
  digitalWrite(LED8, HIGH);
  digitalWrite(LED7, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED3, HIGH);
  delay(650);
  digitalWrite(LED8, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED3, LOW);
  delay(650);
}

void wonderwallBeat(){
  patternAlternate3();
  delay(600);
  patternAlternate3();
  delay(500);
  patternAlternate3();
  delay(20);
  patternAlternate4();
  delay(20);
  patternAlternate3();
  delay(20);
  patternAlternate4();
  delay(20);
  patternAlternate3();
  delay(500);
  patternAlternate4();
  delay(20);
  patternAlternate3();
  delay(20);
  patternAlternate4();
  delay(20);
  patternAlternate3();
  delay(20);
  patternAlternate4();
  delay(500);
  patternAlternate3();
  delay(20);
  patternAlternate4();
  delay(20);
  patternAlternate4();
  delay(20);
  patternAlternate3();
  delay(20);
  patternAlternate3();
  delay(20);
  patternAlternate3();
  delay(200);
}

void playSong(int num){
  if(num == 0){
    digitalWrite(LED5, HIGH);
    digitalWrite(LED6, HIGH);
    digitalWrite(LED7, HIGH);
    digitalWrite(LED8, HIGH);
    delay(200);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(1000);
    clear();
    delay(2000);
    patternAlternate();
    patternAlternate2();
    delay(1500);
    digitalWrite(LED5, HIGH);
    digitalWrite(LED6, HIGH);
    digitalWrite(LED7, HIGH);
    digitalWrite(LED8, HIGH);
    delay(200);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(1000);
    clear();
    delay(2000);
    patternAlternate();
    patternAlternate2();
    delay(1500);
    digitalWrite(LED5, HIGH);
    digitalWrite(LED6, HIGH);
    digitalWrite(LED7, HIGH);
    digitalWrite(LED8, HIGH);
    delay(200);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    clear();
    for(int i = 0; i < 20; i++){
      paranoidBeat();
    }
  }
  if(num == 1){
    for(int j = 0; j < 50; j++){
      sweetBeat();
    }
    
  }
  if(num == 2){
    patternAlternate();
    patternAlternate2();
    delay(600);
    allOn();
    delay(100);
    clear();
    
    for(int k = 0; k<3; k++){
      coldIntroBeat();
    }
    digitalWrite(LED8, HIGH);
    delay(300);
    digitalWrite(LED6, HIGH);
    delay(300);
    digitalWrite(LED7, HIGH);
    delay(300);
    digitalWrite(LED5, HIGH);
    delay(300);
    digitalWrite(LED4, HIGH);
    delay(300);
    digitalWrite(LED2, HIGH);
    delay(300);
    digitalWrite(LED3, HIGH);
    delay(300);
    digitalWrite(LED1, HIGH);
    delay(300);
    clear();
    delay(300);
    allOn();
    delay(400);
    clear();
    delay(500);
    for(int l = 0; l<20; l++){
      coldBeat();
    }
  }
  if(num == 3){
    for(int i = 0; i < 10; i++){
      wonderwallBeat();
    }
  }
  if(num == 4){
    for(int i = 0; i < 45; i++){
      randomLed(1, 200);
    }
    for(int i = 0; i < 45; i++){
      randomLed(2, 200);
    }
    for(int i = 0; i < 80; i++){
      randomLed(3, 100);
    }
    for(int i = 0; i < 1000; i++){
      randomLed(2, 20);
    }
    
  }
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(LED4, OUTPUT);
pinMode(LED5, OUTPUT);
pinMode(LED6, OUTPUT);
pinMode(LED7, OUTPUT);
pinMode(LED8, OUTPUT);
pinMode(BUT1, INPUT);
pinMode(BUT2, INPUT);
pinMode(BUT3, INPUT);
pinMode(BUT4, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(BUT1) == HIGH){
  playSong(0);
}
if(digitalRead(BUT2) == HIGH){
  playSong(1);
}
if(digitalRead(BUT3) == HIGH){
  playSong(2);
}
if(digitalRead(BUT4) == HIGH){
  playSong(3);
}
if(digitalRead(BUT5) == HIGH){
  playSong(4);
}
}
