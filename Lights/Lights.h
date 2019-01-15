// 
// FILE: Lights.h
// VERSION: 1.0
// PURPOSE: Initial Issue of the library
// Created by: Carlos Arenillas
// Nov 2018
//

#ifndef Lights_h
#define Lights_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>


// Pattern types supported:
#define NONE          0
#define FULL_COLOR    1
#define DROP          2
#define DOUBLE_DROP   3
#define SPARKLE       4
#define STROBE        5
#define FADE          6
#define RAINBOW       7
#define SWEEP         8
#define FIRE_H        9
#define FIRE_V        10
#define BREATHE       11
#define FORWARD       1
#define REVERSE       0
#define on            0x05
#define off           0x04


class Message{ 
  private:
    uint8_t content[20];
    uint8_t pos;
  public:
    Message(void);
    void Fill(uint8_t value);
    void Clr(void);
    int Ready(void);
};

class AtRemoteCommand{ 
  private:
    uint8_t Start_delimiter;
    uint16_t Length;
    uint8_t Frame_type;
    uint8_t Frame_ID;
    uint64_t Address64bit;
    uint16_t Address16bit;
    uint8_t RemoteOptions;
    uint16_t Command;
    uint8_t Value;
    uint8_t Checksum;    
  public:
    AtRemoteCommand(void);
    void Send(uint64_t addr64, uint8_t val);
};

class AtCommand{ 
  private:
    uint8_t Start_delimiter;
    uint16_t Length;
    uint8_t Frame_type;
    uint8_t Frame_ID;
    uint16_t Command;
    uint8_t Value;
    uint8_t Checksum;    
  public:
    AtCommand(void);
    void Send(uint8_t val);
};

class TxCommand{ 
  private:
    uint8_t Start_delimiter;
    uint16_t Length;
    uint8_t Frame_type;
    uint8_t Frame_ID;
    uint64_t Address64bit;
    uint8_t Options;
    uint8_t Checksum;
  public:
    TxCommand(void);
    void Send(uint64_t addr64, uint8_t payload[], uint8_t len);
};

class EffectsManager{ 
  private:
	Message m;	
	AtRemoteCommand at;
	TxCommand tx;
  public:
    EffectsManager(void);
    void Change(uint64_t addr64, uint8_t payload[], uint8_t len);
};

// NeoPattern Class - derived from the Adafruit_NeoPixel class  
class NeoPatterns : public Adafruit_NeoPixel{
  private:
    // Member Variables:  
    uint8_t  ActivePattern;  // which pattern is running
    uint8_t Direction;     // direction to run the pattern  
    unsigned long Interval;   // milliseconds between updates
    unsigned long Interval1;   // milliseconds between updates
    unsigned long Interval2;   // milliseconds between updates
    unsigned long Cooling;   //  
    unsigned long Sparking;   // 
    unsigned long lastUpdate; // last update of position  
    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
    uint16_t Change;  //  
	uint32_t DimColor(uint32_t color);	
	void OnComplete(void);
	void Increment(void);
	uint8_t Red(uint32_t color);
	uint8_t Green(uint32_t color);
	uint8_t Blue(uint32_t color);
	void setPixelHeatColor (int Pixel, byte temperature, uint8_t type);
  public:
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type);
    void ColorSet(uint32_t color);
    void Update();
    void FullColor(uint32_t color1);
    void Drop(uint32_t color1, uint8_t interval);
    void DropUpdate();
    void DoubleDrop(uint32_t color1, uint8_t interval);
    void DoubleDropUpdate();
    void Sparkle(uint32_t color1, uint8_t interval);
    void SparkleUpdate();
    void Strobe(uint32_t color1, uint8_t interval1, uint8_t interval2, uint8_t steps);
    void StrobeUpdate();
    void Fade(uint32_t color1, uint8_t interval, uint16_t steps);
    void FadeUpdate();
    void Rainbow(uint8_t interval, uint16_t steps);
    void RainbowUpdate();
    void Sweep(uint32_t color1, uint8_t interval);
    void SweepUpdate();
    void Fire_h(uint32_t color1);
    void Fire_hUpdate();
    void Fire_v(uint8_t interval1, uint8_t interval2, uint8_t cooling, uint8_t sparking);
    void Fire_vUpdate();
    void Breathe(uint32_t color1, uint8_t interval, uint8_t steps);
    void BreatheUpdate();
};

class EffectsHandler{ 
  private:
	uint8_t payload[1];
	TxCommand tx;
	AtCommand at;
	uint64_t MasterAddress;
	uint8_t Trinket_pin;
	uint8_t stick;
	uint8_t effect;
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	uint8_t Interval1;
	uint8_t Interval2;
	uint8_t TotalSteps;
	uint8_t Cooling;
	uint8_t Sparking;
  public:
    EffectsHandler(uint64_t addr64, uint8_t pin);
    void Init(NeoPatterns Stripes[], uint8_t length);
    void Listen(NeoPatterns Stripes[], uint8_t length);
};


#endif	
