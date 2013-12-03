/*
  ADConverter.h - Library for AD converter code.
  Created by Michiel Dommerholt, 2 December 2013.
*/

#include "Arduino.h"
#include "ADConverter.h"

ADConverter::ADConverter(){}

int ADConverter::readAdc(int adcnum, int clockpin, int mosipin, int misopin, int cspin){
  if ((adcnum > 7) or (adcnum < 0)){
    return -1;
  }
  
  digitalWrite(cspin, HIGH);
   
  digitalWrite(clockpin, LOW);  // start clock low
  digitalWrite(cspin, LOW);     // bring CS low
   
  int commandout = adcnum;
  
  commandout |= 0x18;  // start bit + single-ended bit
  commandout <<= 3;    // we only need to send 5 bits here
  for(int i = 0; i < 5; i++){
    if (commandout & 0x80){
      digitalWrite(mosipin, HIGH);
    }else{
      digitalWrite(mosipin, LOW);
    }
    commandout <<= 1;
    digitalWrite(clockpin, HIGH);
    digitalWrite(clockpin, LOW);
  }
   
  int adcout = 0;
  // read in one empty bit, one null bit and 10 ADC bits
  for(int i = 0; i < 12; i++){
    digitalWrite(clockpin, HIGH);
    digitalWrite(clockpin, LOW);
    adcout <<= 1;
    if (digitalRead(misopin)){
      adcout |= 0x1;
    }
  }
  digitalWrite(cspin, HIGH);
  
  adcout >>= 1;       // first bit is 'null' so drop it
  return adcout;
}
