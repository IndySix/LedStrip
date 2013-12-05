/*
  Leds.h - Library for flashing LedStrip code.
  Created by Michiel Dommerholt, 16 Oktober 2013.
*/
#ifndef Leds_h
#define Leds_h

#include "Arduino.h"

class Leds{
  public:
    Leds(int clock, int dataOut, int dataIn, int chipSelect);
    void init(const int ledsCount, const int ledSeparation, const int sensorCount, const int ledsRedPins[], const int ledsGreenPins[], const int sensorPins[], const float sensorTreshold);
    boolean ledsLightSensors();
    void calibrateSensors();
    void startGrind(boolean grindPositive, int startTime, String& startDistance);
    
  private:
    boolean logging;
    
    void setLed(int n, char value);
    int getSensorValue(int sensorID);
    int readAdc(int adcnum, int clockpin, int mosipin, int misopin, int cspin);
    
    int lG[16];
    int lR[16];
    int lS[8];
    int ledStartupValue[16];
    int ledOnTime[8];
    int leds;
    int sensors;
    int lastActivated;
    int ledDelay;
    int ledDistance;
    float tresholdMultiplier;
    
    int SPICLK;  // Orange
    int SPIMISO; // Yellow
    int SPIMOSI; // Blue
    int SPICS;   // Brown
    
    boolean grinding;
    boolean grindDirection;
    int grindStartTime;
    String grindStartDistance;
};

#endif
