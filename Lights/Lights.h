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
#define on            0x05
#define off           0x04


class Message{ 
  private:
    uint8_t content[20];
    uint8_t pos;
  public:
    Message(void);
    Fill(uint8_t value);
    Clr(void);
    Ready(void);
};

class AtCommand{ 
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
    AtCommand(void);
    Send(uint64_t addr64, uint8_t val);
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
    Send(uint64_t addr64, uint8_t payload[], uint8_t len);
};

class EffectsManager{ 
  private:
	Message m;	
	AtCommand at;
	TxCommand tx;
  public:
    EffectsManager(void);
    Change(uint64_t addr64, uint8_t payload[], uint8_t len);
};



#endif	//simpleNTP
