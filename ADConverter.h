/*
  ADConverter.h - Library for AD converter code.
  Created by Michiel Dommerholt, 2 December 2013.
*/

#ifndef ADConverter_h
#define ADConverter_h

#include "Arduino.h"

class ADConverter{
  public:
    ADConverter();
    int readAdc(int adcnum, int clockpin, int mosipin, int misopin, int cspin);
    
  private:
};
#endif



