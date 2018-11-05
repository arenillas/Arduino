#include <Adafruit_NeoPixel.h>

// XBee definition:
uint8_t Trinket_pin = 5;
uint8_t Stick1_pin = 11; //Tira de 52 leds
uint8_t Stick2_pin = 12; //Tira de 08 leds
uint64_t Master_address = 0x0013A20040E86DC9;

// At Command Class
class AtCommand{ 
  private:
    uint8_t Start_delimiter;
    uint16_t Length;
    uint8_t Frame_type;
    uint8_t Frame_ID;
    uint16_t AtCommand;
    uint8_t Value;
    uint8_t Checksum;    
  public:
    AtCommand(){     
    }
    Send(uint8_t val){
      Length = 0x0005;
      uint8_t Frame[Length];
      Start_delimiter = 0x7E;      
      Frame_type = 0x08;
      Frame_ID = 0x00;     
      AtCommand = 0x4435;
      Value = val;
      Frame[0] = Frame_type;
      Frame[1] = Frame_ID;
      Frame[2] = (AtCommand >> 8) & 0xFF;
      Frame[3] = AtCommand & 0xFF;
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

// AT Command Definition
AtCommand at = AtCommand();

// TX Command Definition
TxCommand tx = TxCommand();

// Payload of TX Message
uint8_t payload[] = {0x55};


// Pattern types supported:
enum  pattern { NONE, FULL_COLOR, DROP, DOUBLE_DROP, SPARKLE, STROBE, BREATHE, FIRE_H, FIRE_V, FADE, RAINBOW, SWEEP};
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel{
  public:

  // Member Variables:  
  pattern  ActivePattern;  // which pattern is running
  direction Direction;     // direction to run the pattern
  
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
  
  void (*OnComplete)();  // Callback on completion of pattern
  
  // Constructor - calls base-class constructor to initialize strip
  NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
  :Adafruit_NeoPixel(pixels, pin, type){
    OnComplete = callback;
  }
        
  // Update the pattern
  void Update(){
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

  // Increment the Index and reset at the end
  void Increment(){
    if (Direction == FORWARD){
      Index++;
      if (Index >= TotalSteps){
        Index = 0;
        if (OnComplete != NULL){
          OnComplete(); // call the completion callback
        }
      }      
    }
    
    else{
      --Index;
      if (Index <= 0){
        Index = TotalSteps-1;
        if (OnComplete != NULL){
          OnComplete(); // call the completion callback
        }
      }
    }
  }
  
  // Initialize for a Full Color
  void FullColor(uint32_t color1){
    ActivePattern = FULL_COLOR;    
    Color1 = color1;
    ColorSet(Color1);
  }

  // Update the Full Color Pattern
  //void FullColorUpdate(){
  //  ColorSet(Color1);
  //}

  // Initialize for a Drop
  void Drop(uint32_t color1, uint8_t interval){
    ColorSet(Color(0, 0, 0));
    ActivePattern = DROP;
    Interval = interval;
    TotalSteps = (numPixels() - 1);
    Color1 = color1;
    Index = 0;
  }

  // Update the Drop Pattern
  void DropUpdate(){
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
  void DoubleDrop(uint32_t color1, uint8_t interval){
    ColorSet(Color(0, 0, 0));
    ActivePattern = DOUBLE_DROP;
    Interval = interval;
    TotalSteps = (numPixels() - 1) * 2;
    Color1 = color1;
    Index = 0;
  }

  // Update the Drop Pattern
  void DoubleDropUpdate(){
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
  void Sparkle(uint32_t color1, uint8_t interval){
    ColorSet(Color(0, 0, 0));
    ActivePattern = SPARKLE;
    Color1 = color1;  
    Interval = interval;
    Index = random(numPixels());     
  }
  

  // Update the Sparkle Pattern
  void SparkleUpdate(){    
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
  void Strobe(uint32_t color1, uint8_t interval1, uint8_t interval2, uint8_t steps){
    ActivePattern = STROBE;
    Index = 0;
    Color1 = color1;   
    Interval = interval1;
    Interval1 = interval1;
    Interval2 = interval2;
    TotalSteps = steps;
  } 
  
  // Update the Strobe Pattern
  void StrobeUpdate(){    
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
  void Fade(uint32_t color1, uint16_t steps, uint8_t interval){
    ActivePattern = FADE;
    Interval = interval;
    TotalSteps = steps;
    Color2 = getPixelColor(0);
    Color1 = color1;
    Index = 0;
  }
    
  // Update the Fade Pattern
  void FadeUpdate(){
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
  void Rainbow(uint16_t steps, uint8_t interval){
    ActivePattern = RAINBOW;
    Interval = interval;
    TotalSteps = steps;
    Index = 0;
    Change = 1;
  }  

  // Update the Rainbow Pattern
  void RainbowUpdate(){
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
  void Sweep(uint32_t color1, uint8_t interval){
    ColorSet(Color(0, 0, 0));
    ActivePattern = SWEEP;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color1;
    Index = 0;
  }

  // Update the Sweep Pattern
  void SweepUpdate(){
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
  void Fire_h(uint32_t color1){
    ActivePattern = FIRE_H;
    Color1 = color1;
  }
    
  // Update the Horizontal Fire Pattern
  void Fire_hUpdate(){
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
  void Fire_v(uint32_t color1, uint8_t interval1, uint8_t interval2, uint8_t cooling, uint8_t sparking){
    ActivePattern = FIRE_V;
    Interval = interval1;
    Interval2 = interval2;
    Cooling = cooling;
    Sparking = sparking;
  }
    
  // Update the Vertical Fire Pattern
  void Fire_vUpdate(){
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
  void Breathe(uint32_t color1, uint8_t interval, uint8_t steps){
    ActivePattern = BREATHE;
    Interval = interval;
    Color1 = color1;
    TotalSteps = steps;
    Direction = REVERSE;
    Index = TotalSteps-1;  
  }
    
  // Update the Breathe Pattern
  void BreatheUpdate(){
    
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
  uint32_t DimColor(uint32_t color){
    // Shift R, G and B components one bit to the right
    uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
  }

  // Set all pixels to a color (synchronously)
  void ColorSet(uint32_t color){
    for (int i = 0; i < numPixels(); i++){
      setPixelColor(i, color);
    }
    show();
  }

  // Returns the Red component of a 32-bit color
  uint8_t Red(uint32_t color){
    return (color >> 16) & 0xFF;
  }

  // Returns the Green component of a 32-bit color
  uint8_t Green(uint32_t color){
    return (color >> 8) & 0xFF;
  }

  // Returns the Blue component of a 32-bit color
  uint8_t Blue(uint32_t color){
    return color & 0xFF;
  }

  void setPixelHeatColor (int Pixel, byte temperature, uint8_t type){
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
};

void Stick1Complete();
void Stick2Complete();

// Define some NeoPatterns for the two rings and the stick
//  as well as some completion routines
NeoPatterns Stick1(52, Stick1_pin, NEO_GRB + NEO_KHZ800, &Stick1Complete);
NeoPatterns Stick2(8, Stick2_pin, NEO_GRB + NEO_KHZ800, &Stick2Complete);

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

void setup(){
  
  //#if defined (__AVR_ATtiny85__)
  //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  //#endif
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  pinMode(Trinket_pin, INPUT);
  at.Send(0x04);
  delay(1000);
  
  // Initialize all the pixelStrips
  Stick1.begin();
  Stick2.begin();  

  Stick1.ColorSet(Stick1.Color(0, 0, 0));
  Stick2.ColorSet(Stick2.Color(0, 0, 0));
}

int change_pin(){
    if (digitalRead(13) == LOW){
    return HIGH;
  }
  else{
    return LOW;
  }
}

// Main loop
void loop(){
  int m;
  uint8_t count;
  uint16_t len;
  len = 0;
  m = 0;
    
  if (digitalRead(Trinket_pin) == HIGH){
    tx.Send(Master_address, payload, 1);
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
          switch (stick){
            case 1:
              Stick1.FullColor(Stick1.Color(Red, Green, Blue));
              break;
            case 2:
              Stick2.FullColor(Stick2.Color(Red, Green, Blue));
              break;
          }
          break;
        case 2:
          switch (stick){
            case 1:
              Stick1.Drop(Stick1.Color(Red, Green, Blue), Interval1);
              break;
            case 2:
              Stick2.Drop(Stick2.Color(Red, Green, Blue), Interval1);
              break;
          }
          break;
        case 3:
          switch (stick){
            case 1:
              Stick1.DoubleDrop(Stick1.Color(Red, Green, Blue), Interval1);
              break;
            case 2:
              Stick2.DoubleDrop(Stick2.Color(Red, Green, Blue), Interval1);
              break;
          }
          break;
        case 4:
          switch (stick){
            case 1:
              Stick1.Sparkle(Stick1.Color(Red, Green, Blue), Interval1);
              break;
            case 2:
              Stick2.Sparkle(Stick2.Color(Red, Green, Blue), Interval1);
              break;
          }
          break;
        case 5:
          switch (stick){
            case 1:
              Stick1.Strobe(Stick1.Color(Red, Green, Blue), Interval1, Interval2, TotalSteps);
              break;
            case 2:
              Stick2.Strobe(Stick2.Color(Red, Green, Blue), Interval1, Interval2, TotalSteps);
              break;
          }
          break;
        case 6:
          switch (stick){
            case 1:
              Stick1.Fade(Stick1.Color(Red, Green, Blue), TotalSteps, Interval1);
              break;
            case 2:
              Stick2.Fade(Stick2.Color(Red, Green, Blue), TotalSteps, Interval1);
              break;
          }
          break;
        case 7:
          switch (stick){
            case 1:
              Stick1.Rainbow(TotalSteps,Interval1);
              break;
            case 2:
              Stick2.Rainbow(TotalSteps,Interval1);
              break;
          }
          break;
        case 8:
          switch (stick){
            case 1:
              Stick1.Sweep(Stick1.Color(Red, Green, Blue),Interval1);
              break;
            case 2:
              Stick2.Sweep(Stick2.Color(Red, Green, Blue),Interval1);
              break;
          }
          break;
          case 9:
          switch (stick){
            case 1:
              Stick1.Fire_h(Stick1.Color(Red, Green, Blue));
              break;
            case 2:
              Stick2.Fire_h(Stick2.Color(Red, Green, Blue));
              break;
          }
          break;
        case 10:
          switch (stick){
            case 1:
              //Stick1.Fire_v(Stick1.Color(Red, Green, Blue), Interval1, Interval2, Cooling, Sparking);
              Stick1.Fire_v(Stick1.Color(Red, Green, Blue),50, Interval2, 55, 120);
              break;
            case 2:
              //Stick2.Fire_v(Stick2.Color(Red, Green, Blue),Interval1, Interval2, Cooling, Sparking);
              break;
          }
          break;
        case 11:
          switch (stick){
            case 1:
              Stick1.Breathe(Stick1.Color(Red, Green, Blue), Interval1, TotalSteps);
              break;
            case 2:
              Stick2.Breathe(Stick2.Color(Red, Green, Blue), Interval1, TotalSteps);
              break;
          }
          break;
      }
    }    
    while(Serial.available() == 0);
    Serial.read();
    //delay(10); 
  }
  
  else {
    // And update tbe stick
    Stick1.Update();
    Stick2.Update();
  }  
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Stick 1 Completion Callback
void Stick1Complete(){
  if ((Stick1.ActivePattern == DROP) || (Stick1.ActivePattern == DOUBLE_DROP)){
    Stick1.FullColor(Stick1.Color(0, 0, 0));
    Stick1.Interval = 255;
  }
  else{
    Stick1.ActivePattern = NULL;
    Stick1.Interval = 255;
  }
}

// Stick 2 Completion Callback
void Stick2Complete(){
  if ((Stick2.ActivePattern == DROP) || (Stick2.ActivePattern == DOUBLE_DROP)){
    Stick2.FullColor(Stick2.Color(0, 0, 0));
    Stick2.Interval = 255;
  }
  else{
    Stick2.ActivePattern = NULL;
    Stick2.Interval = 255;
  }
}
