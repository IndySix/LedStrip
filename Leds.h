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
    void init(const int ledCountTop,
              const int ledCountBottom,
              const int ledSeparation,
              const int sensorCount,
              const int multiplexChips,
              const int clockpins[],
              const int mosipins[],
              const int misopins[],
              const int cspins[],
              const float sensorTreshold);
    boolean ledsLightSensors();
    void calibrateSensors();
    void startGrind(boolean grindPositive, int startTime, String& startDistance);
    
  private:
    boolean logging;
    
    void setLed(int n, char value);
    int getSensorValue(int sensorID);
    
    int clock[3];
    int mosi[3];
    int miso[3];
    int cs[3];
    int ledStartupValue[20];
    int ledOnTime[20];
    int ledsTop;
    int ledsBottom;
    int sensors;
    int lastActivated;
    int ledDelay;
    int ledDistance;
    float tresholdMultiplier;
    
    boolean grinding;
    boolean grindDirection;
    int grindStartTime;
    String grindStartDistance;
};

#endif
