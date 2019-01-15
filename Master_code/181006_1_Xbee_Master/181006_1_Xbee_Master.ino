#include <Lights.h>

// Definition of Slave Addresses
uint64_t Slave_address = 0x0013A20040F9E054;

// Effects Manager Creation
EffectsManager eManager = EffectsManager(); 

// Payload of TX Message
// Use this section to compose payload_messages easily
uint8_t StickA = 1;
uint8_t EffectA = FULL_COLOR;
uint8_t RedA = 255;
uint8_t GreenA = 0;
uint8_t BlueA = 0;
uint8_t Interval1A = 0;
uint8_t Interval2A = 0;
uint8_t TotalStepsA = 0;
uint8_t CoolingA = 0;
uint8_t SparkingA = 0;
uint8_t StickB = 2;
uint8_t EffectB = FULL_COLOR;
uint8_t RedB = 25;
uint8_t GreenB = 0;
uint8_t BlueB = 0;
uint8_t Interval1B = 0;
uint8_t Interval2B = 0;
uint8_t TotalStepsB = 0;
uint8_t CoolingB = 0;
uint8_t SparkingB = 0;
//uint8_t payload[] = {StickA, EffectA, RedA, GreenA, BlueA, Interval1A, Interval2A, TotalStepsA, CoolingA, SparkingA};
uint8_t payload1[] = {StickA, FULL_COLOR, 100, 0, 0, 0, 0, 0, 0, 0, StickB, FULL_COLOR, 0, 100, 0, 0, 0, 0, 0, 0};
uint8_t payload1a[] = {StickA, FULL_COLOR, 0, 0, 0, 0, 0, 0, 0, 0, StickB, FULL_COLOR, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t payload2[] = {StickA, DROP, 0, 0, 100, 50, 0, 0, 0, 0, StickB, DROP, 0, 0, 100, 100, 0, 0, 0, 0};
uint8_t payload3[] = {StickA, DOUBLE_DROP, 100, 0, 0, 100, 0, 0, 0, 0, StickB, DOUBLE_DROP, 0, 100, 0, 100, 0, 0, 0, 0};
uint8_t payload5[] = {StickA, SPARKLE, 100, 0, 0, 1, 0, 0, 0, 0, StickB, SPARKLE, 0, 100, 0, 1, 0, 0, 0, 0};
uint8_t payload6[] = {StickA, STROBE, 100, 0, 0, 255, 255, 255, 0, 0, StickB, STROBE, 0, 100, 0, 255, 255, 255, 0, 0};
uint8_t payload7[] = {StickA, FADE, 0, 0, 100, 50, 50, 100, 0, 0, StickB, FADE, 100, 0, 0, 50, 50, 100, 0, 0};
uint8_t payload8[] = {StickA, RAINBOW, 0, 0, 100, 100, 0, 40, 0, 0, StickB, RAINBOW, 100, 0, 0, 50, 0, 20, 0, 0};
uint8_t payload9[] = {StickA, SWEEP, 100, 0, 0, 60, 50, 100, 0, 0, StickB, SWEEP, 0, 100, 0, 60, 50, 100, 0, 0};
uint8_t payload10[] = {StickA, FIRE_H, 100, 0, 0, 100, 50, 100, 0, 0, StickB, FIRE_H, 0, 100, 0, 100, 50, 100, 0, 0};
uint8_t payload11[] = {StickA, FIRE_V, 100, 0, 0, 100, 5, 100, 55, 120, StickB, FIRE_V, 0, 100, 0, 100, 2, 100, 55, 120};
uint8_t payload12[] = {StickA, BREATHE, 0, 0, 100, 20, 0, 100, 0, 0, StickB, BREATHE, 100, 0, 0, 20, 0, 100, 0, 0};


void setup(){  
  Serial.begin(9600);  
  // Pin 13 is the LED. Is used only for debugging inside Lights.cpp
  // It turns on when Trinket tries to stablish communication
  // It turns off when Slave Trinket changes the effect
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); 
}

// Main loop
void loop(){
  //Example of loop
  delay(3000);
  eManager.Change(Slave_address, payload1, 20);
  delay(5000);
  eManager.Change(Slave_address, payload2, 20);
  delay(5000);
  eManager.Change(Slave_address, payload3, 20);
  delay(5000);
  eManager.Change(Slave_address, payload5, 20);
  delay(5000);
  eManager.Change(Slave_address, payload6, 20);
  delay(5000);
  eManager.Change(Slave_address, payload1, 20);
  delay(1000);
  eManager.Change(Slave_address, payload7, 20);
  delay(5000);
  eManager.Change(Slave_address, payload8, 20);
  delay(5000);
  eManager.Change(Slave_address, payload9, 20);
  delay(5000);
  eManager.Change(Slave_address, payload10, 20);
  delay(5000);
  eManager.Change(Slave_address, payload11, 20);
  delay(5000);
  eManager.Change(Slave_address, payload12, 20);
  while(1);  
}

