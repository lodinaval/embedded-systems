// Smart Toilet Flush System (CPE160P final project)
//
// RECONSTRUCTED: the final sketch was lost. This is the Progress Report 1 code
// (EmbeddedProject1.ino, transcribed from its screenshots) updated to the final
// behaviour described in the project documentation:
//   - median echo reading, occupied at <= 1000 us (~17 cm)
//   - 2 s double check before flushing
//   - servo opens the flapper for 10 s (LED on, display shows "FLSH")
//   - relay runs the 5 V pump for 40 s to refill
//   - TM1637 shows the total flush count
// Pins from Progress Report 1 are kept; the TM1637 pins (4, 5) are assumed.

#include <NewPing.h>
#include <Servo.h>
#include <TM1637Display.h>

const int pump_pin = 7;     // 1-channel relay driving the 5 V pump
const int echo_pin = 8;
const int trigger_pin = 9;
const int led_pin = 13;     // red "flushing" LED
const int servo_pin = 6;
const int clk_pin = 4;      // TM1637 CLK (assumed)
const int dio_pin = 5;      // TM1637 DIO (assumed)

const int max_distance = 900;              // cm, NewPing caps this at 500
const unsigned int occupied_echo = 1000;   // us, ~17 cm
const unsigned long confirm_ms = 2000;
const unsigned long flush_ms = 10000;
const unsigned long refill_ms = 40000;

Servo flapper;
NewPing sonar(trigger_pin, echo_pin, max_distance);
TM1637Display display(clk_pin, dio_pin);

bool occupied = false;
int flush_count = 0;

const uint8_t FLSH[] = {
  SEG_A | SEG_E | SEG_F | SEG_G,          // F
  SEG_D | SEG_E | SEG_F,                  // L
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,  // S
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G   // H
};

// Median of 5 pings in microseconds; no echo counts as "nobody there".
unsigned int readEcho(){
  unsigned int echo = sonar.ping_median(5);
  if(echo == 0) echo = max_distance * US_ROUNDTRIP_CM; //fixes if sensor is not working.
  return echo;
}

void flush(){
  //Open flapper for 10 seconds, close flapper
  Serial.println("FLUSHING");
  display.setSegments(FLSH);
  digitalWrite(led_pin, HIGH);
  flapper.write(90);
  delay(flush_ms);
  flapper.write(0);
  digitalWrite(led_pin, LOW);

  //Refill water in tank for 40 seconds
  Serial.println("REFILLING");
  display.showNumberDec(flush_count);
  digitalWrite(pump_pin, HIGH);
  delay(refill_ms);
  digitalWrite(pump_pin, LOW);

  flush_count++;
  display.showNumberDec(flush_count);
  Serial.print("Flush count: ");
  Serial.println(flush_count);
}

void setup() {
  Serial.begin(9600);
  flapper.attach(servo_pin);
  flapper.write(0);
  pinMode(led_pin, OUTPUT);
  pinMode(pump_pin, OUTPUT);
  digitalWrite(pump_pin, LOW);
  display.setBrightness(5);
  display.showNumberDec(flush_count);
}

void loop() {
  unsigned int echo = readEcho();

  //DEBUGGING
  Serial.print("Echo (us): ");
  Serial.println(echo);
  Serial.print("Occupied: ");
  Serial.println(occupied);
  delay(500);

  if(echo <= occupied_echo) occupied = true;
  if(echo > occupied_echo && occupied){
    Serial.println("DOUBLE CHECK");
    delay(confirm_ms); //delays 2 seconds to see if person really left
    if(readEcho() > occupied_echo){
      flush();
      occupied = false;
    }
  }
}
