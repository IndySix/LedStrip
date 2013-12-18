/*
  InfraredSensor.h - Library for infrared sensor code.
  Created by Michiel Dommerholt, 2 December 2013.
*/

#ifndef InfraredSensor_h
#define InfraredSensor_h

#include "Arduino.h"

class InfraredSensor{
  public:
    InfraredSensor(int pin, String sensorDescription, int minTriggers, int triggerDelay, double sensitivity);
    void tick(String *output);
    void init();
    
  private:
    int getValue();
    
    int sensorPin;
    int minimumTriggers;
    int afterTriggerDelay;
    int triggerTime;
    String description;
    
    int distance;
    int triggerCount;
    int treshold;
    
    int numReadings;
    int readings[10];
    int ind;
    int total;
    int average;
};
#endif



