/*
  Leds.h - Library for flashing LedStrip code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Leds_h
#define Leds_h

#include "Arduino.h"

class Leds{
  public:
    Leds(const int ledsCount, const int sensorCount, const int ledsRedPins[], const int ledsGreenPins[], const int sensorPins[], const int controlInputs[], const int modeCount);
    String tick();
    String setMode(int n);
    String ledsOn();
    
  private:
    String ledsLightSensors(int potMeterValue);
    String ledsPotControlled(int valStart, int valEnd);
    String ledsBlink(int delayTime);
    String ledsWalk(int delayTime);
    String ledsKitt(int delayTime);
    String ledsBinary(int delayTime);
    String ledsRandom(int delayTime);
    String ledsToggle(int delayTime);
    String ledsOff();
    void   setLed(int n, char value);
    
    int lG[];
    int lR[];
    int lS[];
    int ledStartupValue[];
    int analogInGrindStart;
    int analogInGrindEnd;
    int buttonPin;
    int modes;
    int leds;
    int sensors;
    
    int ledDelay;
    
    int mode;
    int counter;
    int prevLedsStart;
    int prevLedsEnd;
    int ledOnTime[];
    boolean toggle;
    boolean buttonPressed;
};

#endif
