#include "HX711.h"

#define DOUT0 38
#define CLK0 39
#define DOUT1 40 
#define CLK1 41
#define DOUT2 42 
#define CLK2 43

HX711 scale0(DOUT0, CLK0);
HX711 scale1(DOUT1, CLK1);

float cal0 = 3000;
float cal1 = 3000;

float w0=0;
float w1=0;

unsigned char cmd = 0;
int i = 0;
volatile bool scaleFlag = false;

void setup() {
  
  scale0.set_scale(cal0);
  scale0.tare();
  scale1.set_scale(cal1);
  scale1.tare();
  
  Serial.begin(9600);
}

void loop() {
    if(scaleFlag == true){
  // 7777777777777    
        w0 = scale0.get_units();
        w1 = scale1.get_units();
        Serial.println(w0);
        Serial.println(w1);
        Serial.println("e");
      }
}

void serialEvent(){
  if(Serial.available()){
    cmd = (char)Serial.read();
      switch (cmd){        
        case 's' :
          // 1111111111
          scaleFlag = true;
          break;
          }
   } 
}
