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
    void init(const int ledCount,
              const int ledSeparation,
              const int sensorCount,
              const int multiplexChips,
              const int clockpin,
              const int mosipin,
              const int misopin,
              const int cspins[],
              const float sensorTreshold);
    boolean ledsLightSensors();
    void calibrateSensors();
    void startGrind(boolean grindPositive, int startTime, String& startDistance);
    
  private:
    boolean logging;
    boolean infraredWorkaround;
    
    void setLed(int n, uint32_t c);
    void sensorActivated(int n, uint32_t c);
    int getSensorValue(int sensorID);
    void buildOutput();
    
    int clock;
    int mosi;
    int miso;
    int cs[3];
    int ledStartupValue[20];
    int ledOnTime[20];
    int leds;
    int sensors;
    int lastActivated;
    int lastActivatedTime;
    int ledDelay;
    int ledDistance;
    float tresholdMultiplier;
    
    boolean grinding;
    boolean grindDirection;
    int grindStartTime;
    String grindStartDistance;
};

#endif
