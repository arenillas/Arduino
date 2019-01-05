//
// FILE: Lights.cpp
// VERSION: 1.0
// PURPOSE: Initial Issue of the library
// Created by: Carlos Arenillas
// Nov 2018
//

#include "Lights.h"


// Change Pin Value Function
int change_pin(){
    if (digitalRead(13) == LOW){
    return HIGH;
  }
  else{
    return LOW;
  }
}


// Message  Class
Message::Message(){
  pos = 0;     
}
Message::Fill(uint8_t value){
  content[pos] = value;
 pos++;
}
Message::Clr(){
  for (int i = 0; i < 20; i++) { 
	content[i] = 0;
	pos = 0;
  }
}
Message::Ready(){
  if ((content[11] == 0x55)){
	return 1;
  }
  else{
	return 0;
  }
}


// At Command Class
AtCommand::AtCommand(){
}
AtCommand::Send(uint64_t addr64, uint8_t val){
  Length = 0x0010;
  uint8_t Frame[Length];
  Start_delimiter = 0x7E;      
  Frame_type = 0x17;
  Frame_ID = 0x00;
  Address64bit = addr64;
  Address16bit = 0xFFFE;
  RemoteOptions = 0x02;
  Command = 0x4435;
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
  Frame[13] = (Command >> 8) & 0xFF;
  Frame[14] = Command & 0xFF;
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


// Tx Command Class
TxCommand::TxCommand(){
}
TxCommand::Send(uint64_t addr64, uint8_t payload[], uint8_t len){
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

// Message  Class
EffectsManager::EffectsManager(){
	// Objects Creation
	Message m = Message();	
	AtCommand at = AtCommand();
	TxCommand tx = TxCommand();
}
EffectsManager::Change(uint64_t addr64, uint8_t payload[], uint8_t len){
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
	    digitalWrite(13, change_pin());
	}
	m.Clr();
}
