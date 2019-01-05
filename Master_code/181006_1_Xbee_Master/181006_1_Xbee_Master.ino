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
    Message(){
      pos = 0;     
    }
    Fill(uint8_t value){
      content[pos] = value;
     pos++;
    }
    Clr(){
      for (int i = 0; i < 20; i++) { 
        content[i] = 0;
        pos = 0;
      }
    }
    Ready(){
      if ((content[11] == 0x55)){
        return 1;
      }
      else{
        return 0;
      }
    }
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
    uint16_t AtCommand;
    uint8_t Value;
    uint8_t Checksum;    
  public:
    AtCommand(){     
    }
    Send(uint64_t addr64, uint8_t val){
      Length = 0x0010;
      uint8_t Frame[Length];
      Start_delimiter = 0x7E;      
      Frame_type = 0x17;
      Frame_ID = 0x00;
      Address64bit = addr64;
      Address16bit = 0xFFFE;
      RemoteOptions = 0x02;
      AtCommand = 0x4435;
      Value = val;
      Frame[0] = Frame_type;
      Frame[1] = Frame_ID;
      Frame[2] = (Address64bit >> 56) & 0xFF;
      Frame[3] = (Address64bit >> 48) & 0xFF;
      Frame[4] = (Address64bit >> 40) & 0xFF;
      Frame[5] = (Address64bit >> 32) & 0xFF;
      Frame[6] = (Address64bit >> 24) & 0xFF;
      Frame[7] = (Address64bit >> 16) & 0xFF;
      Frame[8] = (Address64bit >> 8) & 0xFF;
      Frame[9] = Address64bit & 0xFF;
      Frame[10] = (Address16bit >> 8) & 0xFF;
      Frame[11] = Address16bit & 0xFF;
      Frame[12] = RemoteOptions;
      Frame[13] = (AtCommand >> 8) & 0xFF;
      Frame[14] = AtCommand & 0xFF;
      Frame[15] = Value;
      Checksum = 0;
      for (uint16_t i = 0; i < Length; i++){
        Checksum += Frame[i];
      }       
      Checksum = 0xFF - Checksum;
      Serial.write((byte)Start_delimiter);        
      Serial.write((byte)((Length >> 8) & 0xFF));
      Serial.write((byte)(Length & 0xFF));
      for (int i = 0; i < Length; i++){
        Serial.write((byte)Frame[i]);
      }
      Serial.write((byte)Checksum);
    }
};


// Tx Command Class
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
    TxCommand(){     
    }
    Send(uint64_t addr64, uint8_t payload[], uint8_t len){
      Length = len + 11;
      uint8_t Frame[Length];
      Start_delimiter = 0x7E;      
      Frame_type = 0x00;
      Frame_ID = 0x00;
      Address64bit = addr64;
      Options = 0x00;
      Frame[0] = Frame_type;
      Frame[1] = Frame_ID;
      Frame[2] = (Address64bit >> 56) & 0xFF;
      Frame[3] = (Address64bit >> 48) & 0xFF;
      Frame[4] = (Address64bit >> 40) & 0xFF;
      Frame[5] = (Address64bit >> 32) & 0xFF;
      Frame[6] = (Address64bit >> 24) & 0xFF;
      Frame[7] = (Address64bit >> 16) & 0xFF;
      Frame[8] = (Address64bit >> 8) & 0xFF;
      Frame[9] = Address64bit & 0xFF;
      Frame[10] = Options;
      for (uint8_t i = 0; i < len; i++){
        Frame [11+i] = payload[i];
      }
      Checksum = 0;
      for (uint16_t i = 0; i < Length; i++){
        Checksum += Frame[i];
      }       
      Checksum = 0xFF - Checksum;
      Serial.write((byte)Start_delimiter);        
      Serial.write((byte)((Length >> 8) & 0xFF));
      Serial.write((byte)(Length & 0xFF));
      for (int i = 0; i < Length; i++){
        Serial.write((byte)Frame[i]);
      }
      Serial.write((byte)Checksum);
    }
};


uint64_t Slave_address = 0x0013A20040F9E054;

// Payload of TX Message
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
uint8_t payload1[] = {StickA, 1, 0, 0, 100, 100, 100, 100, CoolingA, SparkingA};
uint8_t payload2[] = {StickA, DOUBLE_DROP, 255, 96, 12, 100, 100, 100, CoolingA, SparkingA, StickB, EffectB, 0, 0, 100, Interval1B, Interval2B, TotalStepsB, CoolingB, SparkingB};
uint8_t payload3[] = {StickB, EffectB, 0, 0, 0, Interval1B, Interval2B, TotalStepsB, CoolingB, SparkingB};
uint8_t payload4[] = {StickA, EffectA, 0, 0, 100, Interval1A, Interval2A, TotalStepsA, CoolingA, SparkingA, StickB, EffectB, 0, 0, 0, Interval1B, Interval2B, TotalStepsB, CoolingB, SparkingB};


Message m = Message();

// AT Command Definition
AtCommand at = AtCommand();

// TX Command Definition
TxCommand tx = TxCommand();

int change_pin(){
    if (digitalRead(13) == LOW){
    return HIGH;
  }
  else{
    return LOW;
  }
}

int Change_effect(uint64_t addr64, uint8_t payload[], uint8_t len){
  digitalWrite(13, change_pin());
  at.Send(addr64, on);    
  while (Serial.read() != 0x80);
  m.Fill(0x80); 
  while (Serial.available() < 12);
  while (Serial.available()){
    m.Fill(Serial.read());
  }
  if (m.Ready()){
    tx.Send(addr64, payload, len);
    //digitalWrite(13, change_pin());
  }
  m.Clr();
  return 0;
}

void setup(){  
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); 
}

// Main loop
void loop(){
  delay(3000);

  Change_effect(Slave_address, payload1, 10);
  delay(5000);

  Change_effect(Slave_address, payload2, 20);
  delay(5000);

  Change_effect(Slave_address, payload3, 10);
  delay(5000); 

  while(1);  
}


