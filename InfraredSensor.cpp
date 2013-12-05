/*
  InfraredSensor.h - Library for infrared sensor code.
  Created by Michiel Dommerholt, 2 December 2013.
*/

#include "Arduino.h"
#include "InfraredSensor.h"

InfraredSensor::InfraredSensor(int pin, String sensorDescription, int minTriggers, int triggerDelay, double sensitivity){
  sensorPin             = pin;
  minimumTriggers       = minTriggers;  // Minimal amount of triggers required to activate
  afterTriggerDelay     = triggerDelay; // Delay after being triggered.
  description           = sensorDescription;
  sensitivityMultiplier = sensitivity;

  distance        = 0;
  triggerCount    = 0;
  
  numReadings = 10;
  for (int i = 0; i < numReadings; i++){
    readings[i] = 0;
  }
}

void InfraredSensor::init(){
  treshold = 0;
  for(int i = 0; i < 25; i++){
    treshold += getValue();
    delay(10);
  }
  treshold = ((double)treshold / 25.0) / 0.9;
}

int InfraredSensor::getValue(){
  return analogRead(sensorPin);
}

void InfraredSensor::tick(String *output){
  int distance = getValue();
      
  total -= readings[ind];
  readings[ind] = distance;
  total += readings[ind];  
  ind++;
  if (ind >= numReadings){
    ind = 0;
  }
  average = total / numReadings;

  if (average > treshold){
    triggerCount++;
    if(triggerCount >= minimumTriggers && triggerTime + afterTriggerDelay < millis()){
//      Serial.print("Sensor: ");
//      Serial.print(description);
//      Serial.print(". Distance: ");
//      Serial.print(distance);
//      Serial.print(" Average: ");
//      Serial.print(average);
//      Serial.print(" Treshold: ");
//      Serial.print(treshold);
//      Serial.print(" Time: ");
//      Serial.println(millis());
      Serial.println("A"); // Signal "Grind Activated"
      triggerCount = 0;
      triggerTime = millis();
      output[0] = description;
      output[1] = (String)average;
    }
  }else{
    triggerCount = 0;
  }
}

