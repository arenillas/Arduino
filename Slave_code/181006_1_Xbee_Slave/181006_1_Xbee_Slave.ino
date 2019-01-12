#include <Lights.h>

// Definition of Master Addresses
uint64_t Master_address = 0x0013A20040E86DC9;

// Trinket Pin which is connected to XBee pin 15
uint8_t Trinket_pin = 5;

// Effects Handler Creation
EffectsHandler eHandler = EffectsHandler(Master_address, Trinket_pin); 

// Neopixel Creation
#define s_number 2 //Number of Neopixel Strips
// Number of leds in each Neopixel Strip
#define Stick1_pixels 52 //Tira de 52 leds
#define Stick2_pixels 8 //Tira de 08 leds
// Trinket pins used for Neopixel Strips control
#define Stick1_pin 11 //Tira de 52 leds
#define Stick2_pin 12 //Tira de 08 leds
NeoPatterns Stripes[s_number] = {NeoPatterns(Stick1_pixels, Stick1_pin, NEO_GRB + NEO_KHZ800), NeoPatterns(Stick2_pixels, Stick2_pin, NEO_GRB + NEO_KHZ800)};

void setup(){  
  Serial.begin(9600);   
  // Pin 13 is the LED. Is used only for debugging inside Lights.cpp
  // It turns on when Trinket tries to stablish communication
  // It turns off when Slave Trinket changes the effect
  pinMode(13, OUTPUT);
  pinMode(Trinket_pin, INPUT);  
  eHandler.Init(Stripes, s_number);
  delay(1000); 
}

// Main loop
void loop(){
  eHandler.Listen(Stripes, s_number);
}


