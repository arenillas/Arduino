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
uint8_t payload1[] = {StickA, 1, 100, 0, 0, 100, 100, 100, CoolingA, SparkingA};
uint8_t payload2[] = {StickA, BREATHE, 0, 100, 0, 30, 8, 100, CoolingA, SparkingA, StickB, EffectB, 0, 0, 100, Interval1B, Interval2B, TotalStepsB, CoolingB, SparkingB};
uint8_t payload3[] = {StickB, EffectB, 0, 0, 0, Interval1B, Interval2B, TotalStepsB, CoolingB, SparkingB};
uint8_t payload4[] = {StickA, EffectA, 0, 0, 100, Interval1A, Interval2A, TotalStepsA, CoolingA, SparkingA, StickB, EffectB, 0, 0, 0, Interval1B, Interval2B, TotalStepsB, CoolingB, SparkingB};


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
  eManager.Change(Slave_address, payload1, 10);
  delay(5000);
  eManager.Change(Slave_address, payload2, 20);
  delay(5000);
  eManager.Change(Slave_address, payload3, 10);
  delay(5000);
  while(1);  
}

