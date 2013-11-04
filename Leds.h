/*
  Leds.h - Library for flashing LedStrip code.
  Created by Michiel Dommerholt, 16 Oktober 2013.
*/
#ifndef Leds_h
#define Leds_h

#include "Arduino.h"

class Leds{
  public:
    Leds();
    void init(const int ledsCount, const int ledSeparation, const int sensorCount, const int ledsRedPins[], const int ledsGreenPins[], const int sensorPins[], const float sensorTreshold);
    void ledsLightSensors();
    void calibrateSensors();
    void startGrind(int timestamp);
    
  private:
    boolean logging;
    
    void setLed(int n, char value);
    
    int lG[16];
    int lR[16];
    int lS[8];
    int ledStartupValue[16];
    int ledOnTime[8];
    int leds;
    int sensors;
    int lastActivated;
    int ledDelay;
    int grindStart;
    int ledDistance;
    float tresholdMultiplier;
    
    boolean grinding;
};

#endif
