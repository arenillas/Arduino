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


// At Remote Command Class
AtRemoteCommand::AtRemoteCommand(){
}
AtRemoteCommand::Send(uint64_t addr64, uint8_t val){
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


// At Command Class
AtCommand::AtCommand(){     
}
AtCommand::Send(uint8_t val){
  Length = 0x0005;
  uint8_t Frame[Length];
  Start_delimiter = 0x7E;      
  Frame_type = 0x08;
  Frame_ID = 0x00;     
  Command = 0x4435;
  Value = val;
  Frame[0] = Frame_type;
  Frame[1] = Frame_ID;
  Frame[2] = (Command >> 8) & 0xFF;
  Frame[3] = Command & 0xFF;
  Frame[4] = Value;
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


// EffectsManager Class
EffectsManager::EffectsManager(){
	// Objects Creation
	Message m = Message();	
	AtRemoteCommand at = AtRemoteCommand();
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



// NeoPattern Class - derived from the Adafruit_NeoPixel class  
// Constructor - calls base-class constructor to initialize strip
NeoPatterns::NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type)
:Adafruit_NeoPixel(pixels, pin, type){
	// Initialize everything and prepare to start
	int effect;
	int stick;
	int Red = 0;
	int Green = 0;
	int Blue = 0;
	int count = 0;
	int Interval1 = 1;
	int Interval2 = 1;
	int TotalSteps = 1;
	int Cooling = 1;
	int Sparking = 1;
}
        
// Update the pattern
NeoPatterns::Update(){
	if((millis() - lastUpdate) > Interval){
	  lastUpdate = millis();
	  switch(ActivePattern){
		//case FULL_COLOR:
		//  FullColorUpdate();
		//  break;
		case DROP:
			DropUpdate();
			break;
		case DOUBLE_DROP:
			DoubleDropUpdate();
			break;
		case SPARKLE:
			SparkleUpdate();
			break;
		case STROBE:
			StrobeUpdate();
			break;
		case FADE:
			FadeUpdate();
			break;
		case RAINBOW:
			RainbowUpdate();
			break;
		case SWEEP:
			SweepUpdate();
			break;
		case FIRE_H:
			Fire_hUpdate();
			break;
		case FIRE_V:
			Fire_vUpdate();
			break;
		case BREATHE:
			BreatheUpdate();
			break;
		default:
			break;
		}
	}
}


NeoPatterns::OnComplete(){
	// Stick Completion Callback
	if ((ActivePattern == DROP) || (ActivePattern == DOUBLE_DROP)){
		FullColor(Color(0, 0, 0));
		Interval = 255;
	}
	else{
		ActivePattern = NULL;
		Interval = 255;
	}
}


// Increment the Index and reset at the end
NeoPatterns::Increment(){
	if (Direction == FORWARD){
		Index++;
		if (Index >= TotalSteps){
			Index = 0;
			OnComplete(); // call the completion callback
		}      
	}
	else{
		--Index;
		if (Index <= 0){
			Index = TotalSteps-1;
			OnComplete(); // call the completion callback
		}
	}
}

// Initialize for a Full Color
NeoPatterns::FullColor(uint32_t color1){
	ActivePattern = FULL_COLOR;    
	Color1 = color1;
	ColorSet(Color1);
}

// Update the Full Color Pattern
//void FullColorUpdate(){
//  ColorSet(Color1);
//}

// Initialize for a Drop
NeoPatterns::Drop(uint32_t color1, uint8_t interval){
	ColorSet(Color(0, 0, 0));
	ActivePattern = DROP;
	Interval = interval;
	TotalSteps = (numPixels() - 1);
	Color1 = color1;
	Index = 0;
}

// Update the Drop Pattern
NeoPatterns::DropUpdate(){
	for (int i = 0; i < numPixels(); i++){
		if (i == Index){
			setPixelColor(i, Color1);
		}
		else if (i == TotalSteps *2 - Index){
			setPixelColor(i, Color1);
		}
		else{
			setPixelColor(i, DimColor(getPixelColor(i)));
		}
	}
	show();
	Increment();
} 

// Initialize for a Double Drop
NeoPatterns::DoubleDrop(uint32_t color1, uint8_t interval){
	ColorSet(Color(0, 0, 0));
	ActivePattern = DOUBLE_DROP;
	Interval = interval;
	TotalSteps = (numPixels() - 1) * 2;
	Color1 = color1;
	Index = 0;
}

// Update the Drop Pattern
NeoPatterns::DoubleDropUpdate(){
	for (int i = 0; i < numPixels(); i++){
		if (i == Index){
			setPixelColor(i, Color1);
		}
		else if (i == TotalSteps - Index){
			setPixelColor(i, Color1);
		}
		else{
			setPixelColor(i, DimColor(getPixelColor(i)));
		}
	}
	show();
	Increment();
} 

// Initialize for a Sparkle
NeoPatterns::Sparkle(uint32_t color1, uint8_t interval){
	ColorSet(Color(0, 0, 0));
	ActivePattern = SPARKLE;
	Color1 = color1;  
	Interval = interval;
	Index = random(numPixels());     
}

// Update the Sparkle Pattern
NeoPatterns::SparkleUpdate(){    
	if (getPixelColor(Index) == 0){
		Index = random(numPixels());
		setPixelColor(Index, Color1);
	}
	else{
		setPixelColor(Index, 0);
	}
	show(); 
}

// Initialize for a Strobe
NeoPatterns::Strobe(uint32_t color1, uint8_t interval1, uint8_t interval2, uint8_t steps){
	ActivePattern = STROBE;
	Index = 0;
	Color1 = color1;   
	Interval = interval1;
	Interval1 = interval1;
	Interval2 = interval2;
	TotalSteps = steps;
} 

// Update the Strobe Pattern
NeoPatterns::StrobeUpdate(){    
	for(int i=0; i< numPixels(); i++){
		if (getPixelColor(i) == 0){
			setPixelColor(i, Color1);
			Interval = Interval1;        
		}
		else{
			setPixelColor(i, 0);
			if (Index < TotalSteps -1){
				Interval = Interval1;
				Increment();
			}
			else{
				Interval = Interval2;
				Index = 0;            
			}
		}
	}
	show();    
}

// Initialize for a Fade
NeoPatterns::Fade(uint32_t color1, uint16_t steps, uint8_t interval){
	ActivePattern = FADE;
	Interval = interval;
	TotalSteps = steps;
	Color2 = getPixelColor(0);
	Color1 = color1;
	Index = 0;
}

// Update the Fade Pattern
NeoPatterns::FadeUpdate(){
	// Calculate linear interpolation between Color1 and Color2
	// Optimise order of operations to minimize truncation error
	uint8_t red = ((Red(Color2) * (TotalSteps - Index)) + (Red(Color1) * Index)) / TotalSteps;
	uint8_t green = ((Green(Color2) * (TotalSteps - Index)) + (Green(Color1) * Index)) / TotalSteps;
	uint8_t blue = ((Blue(Color2) * (TotalSteps - Index)) + (Blue(Color1) * Index)) / TotalSteps;

	ColorSet(Color(red, green, blue));
	//if (Index < TotalSteps -1){
	Increment();
	//}
}

// Initialize for a Rainbow
NeoPatterns::Rainbow(uint16_t steps, uint8_t interval){
	ActivePattern = RAINBOW;
	Interval = interval;
	TotalSteps = steps;
	Index = 0;
	Change = 1;
}  

// Update the Rainbow Pattern
NeoPatterns::RainbowUpdate(){
	// Calculate linear interpolation between Color1 and Color2
	// Optimise order of operations to minimize truncation error
	switch (Change){
		case 1:
			Color1 = 16776960;
			Color2 = 16711680;
			break;
		case 2:
			Color1 = 16711680;
			Color2 = 7874700;
			break;
		case 3:
			Color1 = 7874700;
			Color2 = 255;
			break;
		case 4:
			Color1 = 255;
			Color2 = 65535;
			break;
		case 5:
			Color1 = 65535;
			Color2 = 65280;
			break;
		case 6:
			Color1 = 65280;
			Color2 = 16776960;
			break;
	}

	uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
	uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
	uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

	ColorSet(Color(red, green, blue));
	if (Index < TotalSteps -1){
		Increment();
	}
	if (Index == TotalSteps -1){
		if (Change == 6){
			Change = 1;
		}
		else Change++;
		Index = 0;
	}     
}

// Initialize for a Sweep
NeoPatterns::Sweep(uint32_t color1, uint8_t interval){
	ColorSet(Color(0, 0, 0));
	ActivePattern = SWEEP;
	Interval = interval;
	TotalSteps = numPixels();
	Color1 = color1;
	Index = 0;
}

// Update the Sweep Pattern
NeoPatterns::SweepUpdate(){
	for (int i = 0; i < numPixels(); i++){
		if (i == Index){
			setPixelColor(i, Color1);
		}
	}      
	show();
	if (Index < TotalSteps -1){
		Increment();
	}
}

// Initialize for a Horizontal Fire
NeoPatterns::Fire_h(uint32_t color1){
	ActivePattern = FIRE_H;
	Color1 = color1;
}

// Update the Horizontal Fire Pattern
NeoPatterns::Fire_hUpdate(){
	for(int x = 0; x < numPixels(); x++){
		int flicker = random(0,40);
		int r1 = Red(Color1)-flicker;
		int g1 = Green(Color1)-flicker;
		int b1 = Blue(Color1)-flicker;
		if(g1<0) g1=0;
		if(r1<0) r1=0;
		if(b1<0) b1=0;
		setPixelColor(x,r1,g1,b1);
	}
	show();
	Interval = random(50,150);
} 

// Initialize for a Vertical Fire
NeoPatterns::Fire_v(uint32_t color1, uint8_t interval1, uint8_t interval2, uint8_t cooling, uint8_t sparking){
	ActivePattern = FIRE_V;
	Interval = interval1;
	Interval2 = interval2;
	Cooling = cooling;
	Sparking = sparking;
}

// Update the Vertical Fire Pattern
NeoPatterns::Fire_vUpdate(){
	static byte heat[52];
	int cooldown;  
	// Step 1.  Cool down every cell a little
	for(int i = 0; i < numPixels(); i++){
		cooldown = random(0, ((Cooling * 10) / numPixels()) + 2);
		if(cooldown>heat[i]){
			heat[i]=0;
		} 
		else{
			heat[i]=heat[i]-cooldown;
		}
	}  
	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for(int k= numPixels() - 1; k >= 2; k--){
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
	}    
	// Step 3.  Randomly ignite new 'sparks' near the bottom
	if(random(255) < Sparking ){
		int y = random(7);
		heat[y] = heat[y] + random(160,255);
		//heat[y] = random(160,255);
	}
	// Step 4.  Convert heat to LED colors
	for( int j = 0; j < numPixels(); j++) {
		setPixelHeatColor(j, heat[j], Interval2);
	}    
	show();
}

// Initialize for a Breathe
NeoPatterns::Breathe(uint32_t color1, uint8_t interval, uint8_t steps){
	ActivePattern = BREATHE;
	Interval = interval;
	Color1 = color1;
	TotalSteps = steps;
	Direction = REVERSE;
	Index = TotalSteps-1;  
}

// Update the Breathe Pattern
NeoPatterns::BreatheUpdate(){
	float  factor = sin(Index * PI / TotalSteps);
	uint8_t red = Red(Color1) * factor;
	uint8_t green = Green(Color1) * factor;
	uint8_t blue = Blue(Color1) * factor;  
	
	//uint8_t red = (Red(Color1) * (TotalSteps - Index)) / TotalSteps;
	//uint8_t green = (Green(Color1) * (TotalSteps - Index)) / TotalSteps;
	//uint8_t blue = (Blue(Color1) * (TotalSteps - Index)) / TotalSteps;  

	ColorSet(Color(red, green, blue));
	Increment();
	if (Index == 1 && Direction == REVERSE){
		Direction = FORWARD;    
	}
	else if (Index == (TotalSteps - 1) && Direction == FORWARD){
		Direction = REVERSE;
	}
} 

// Calculate 50% dimmed version of a color (used by DropUpdate)
uint32_t NeoPatterns::DimColor(uint32_t color){
	// Shift R, G and B components one bit to the right
	uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
	return dimColor;
}

// Set all pixels to a color (synchronously)
NeoPatterns::ColorSet(uint32_t color){
	for (int i = 0; i < numPixels(); i++){
		setPixelColor(i, color);
	}
	show();
}

// Returns the Red component of a 32-bit color
uint8_t NeoPatterns::Red(uint32_t color){
	return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t NeoPatterns::Green(uint32_t color){
	return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t NeoPatterns::Blue(uint32_t color){
	return color & 0xFF;
}

NeoPatterns::setPixelHeatColor (int Pixel, byte temperature, uint8_t type){
	// Scale 'heat' down from 0-255 to 0-191
	byte t192 = round((temperature/255.0)*191);

	// calculate ramp up from
	byte heatramp = t192 & 0x3F; // 0..63
	heatramp <<= 2; // scale up to 0..252

	// figure out which third of the spectrum we're in:
	if( t192 > 0x80){                     // hottest
		switch (type){
			case 1:
				setPixelColor(Pixel, 255, 255, heatramp);
				break;
			case 2:
				setPixelColor(Pixel, heatramp, 255, 255);
				break;          
			case 3:
				setPixelColor(Pixel, 255, heatramp, 255);
				break;
			case 4:
				setPixelColor(Pixel, heatramp, 255, 255);
				break;
			case 5:
				setPixelColor(Pixel, 255, heatramp, 255);
				break;
			default:
				setPixelColor(Pixel, 255, 255, heatramp);
				break;  
		}      
	} 
	else if( t192 > 0x40 ){             // middle
		switch (type){
			case 1:
				setPixelColor(Pixel, heatramp, 255, 0);
				break;
			case 2:
				setPixelColor(Pixel, 0, 255, heatramp);
				break;          
			case 3:
				setPixelColor(Pixel, heatramp, 0, 255);
				break;
			case 4:
				setPixelColor(Pixel, 0, heatramp, 255);
				break;
			case 5:
				setPixelColor(Pixel, 255, 0, heatramp);
				break;
			default:
				setPixelColor(Pixel, 255, heatramp, 0);
				break;
		}
	} 
	else{                               // coolest
		switch (type){
			case 1:
				setPixelColor(Pixel, 0, heatramp, 0);
				break;
			case 2:
				setPixelColor(Pixel, 0, heatramp, 0);
				break;          
			case 3:
				setPixelColor(Pixel, 0, 0, heatramp);
				break;
			case 4:
				setPixelColor(Pixel, 0, 0, heatramp);
				break;
			case 5:
				setPixelColor(Pixel, heatramp, 0, 0);
				break;
			default:
				setPixelColor(Pixel, heatramp, 0, 0);
				break;
		}
	}
}


// Effects Handler Class
EffectsHandler::EffectsHandler(uint64_t addr64, uint8_t pin){
	// Payload of TX Message
	payload[0] = {0x55};
	// Objects Creation
	TxCommand tx = TxCommand();
	AtCommand at = AtCommand();
	MasterAddress = addr64;
	Trinket_pin = pin;
}


EffectsHandler::Init(NeoPatterns Stripes[], uint8_t length){
	for (int i=1; i <= length; i++){
		Stripes[i-1].begin();
		Stripes[i-1].ColorSet(Stripes[i-1].Color(0, 0, 0));
	}
	at.Send(0x04);	
}


EffectsHandler::Listen(NeoPatterns Stripes[], uint8_t length){
	
	int m;
	uint8_t count;
	uint16_t len;
	len = 0;
	m = 0;

	if (digitalRead(Trinket_pin) == HIGH){
		tx.Send(MasterAddress, payload, 1);
		at.Send(0x04);
		while (m != 0x80){
			if (m != -1){
				len = ((len & 0xFF) << 8) + m;
			}      
			m = Serial.read();
		}
		digitalWrite(13, change_pin());
		for (int i = 0; i < 10; i++) {
			while(Serial.available() == 0);
			Serial.read();
		}  
		for (int i = 0; i < ((len - 11) / 10); i++) {
			for (int j = 0; j < 10; j++) {
				switch (j){
					case 0:
					   digitalWrite(13, change_pin()); 
					   while(Serial.available() == 0);
					   stick = Serial.read();   
					   break;
					case 1:
					   while(Serial.available() == 0);
					   effect = Serial.read();
					   break;
					case 2:
					   while(Serial.available() == 0);
					   Red = Serial.read(); 
					   break;
					case 3:
					   while(Serial.available() == 0);
					   Green = Serial.read();
					   break;
					case 4:
					   while(Serial.available() == 0);
					   Blue = Serial.read(); 
					   break;
					case 5:
					   while(Serial.available() == 0);
					   Interval1 = Serial.read(); 
					   break;
					case 6:
					   while(Serial.available() == 0);
					   Interval2 = Serial.read(); 
					   break;
					case 7:
					   while(Serial.available() == 0);
					   TotalSteps = Serial.read();
					   break;
					case 8:
					   while(Serial.available() == 0);
					   Cooling = Serial.read();
					   break;
					case 9:
					   while(Serial.available() == 0);
					   Sparking = Serial.read();
					   break;                      
			   }
			}
			switch (effect){
				case 1:
					Stripes[stick-1].FullColor(Stripes[stick-1].Color(Red, Green, Blue));					
					break;
				case 2:
					Stripes[stick-1].Drop(Stripes[stick-1].Color(Red, Green, Blue), Interval1);
					break;
				case 3:
					Stripes[stick-1].DoubleDrop(Stripes[stick-1].Color(Red, Green, Blue), Interval1);
					break;
				case 4:
					Stripes[stick-1].Sparkle(Stripes[stick-1].Color(Red, Green, Blue), Interval1);
					break;
				case 5:
					Stripes[stick-1].Strobe(Stripes[stick-1].Color(Red, Green, Blue), Interval1, Interval2, TotalSteps);
					break;
				case 6:
					Stripes[stick-1].Fade(Stripes[stick-1].Color(Red, Green, Blue), TotalSteps, Interval1);
					break;
				case 7:
					Stripes[stick-1].Rainbow(TotalSteps,Interval1);
					break;
				case 8:
					Stripes[stick-1].Sweep(Stripes[stick-1].Color(Red, Green, Blue),Interval1);
					break;
				case 9:
					Stripes[stick-1].Fire_h(Stripes[stick-1].Color(Red, Green, Blue));
					break;
				case 10:
					Stripes[stick-1].Fire_v(Stripes[stick-1].Color(Red, Green, Blue), Interval1, Interval2, Cooling, Sparking);
					// Valores que funcionan bien
					// Stripes[stick-1].Fire_v(Stripes[stick-1].Color(Red, Green, Blue),50, Interval2, 55, 120);
					break;
				case 11:
					Stripes[stick-1].Breathe(Stripes[stick-1].Color(Red, Green, Blue), Interval1, TotalSteps);
					break;
			}
		}    
		while(Serial.available() == 0);
		Serial.read();
		//delay(10); 
	}	
	else {
		// And update tbe stick
		for (int i=1; i <= length; i++){
			Stripes[i-1].Update();
		}
	}  
}
