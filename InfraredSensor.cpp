/*
  InfraredSensor.h - Library for infrared sensor code.
  Created by Michiel Dommerholt, 2 December 2013.
*/

#include "Arduino.h"
#include "InfraredSensor.h"

InfraredSensor::InfraredSensor(int pin, String sensorDescription, int minTriggers, int triggerDelay, double sensitivity){
  sensorPin         = pin;               // Sensor pin
  minimumTriggers   = minTriggers;       // Minimal amount of triggers required to activate
  afterTriggerDelay = triggerDelay;      // Delay after being triggered.
  description       = sensorDescription; // Sensor name
  distance          = 0;
  triggerCount      = 0;
  
  numReadings = 10;
  for (int i = 0; i < numReadings; i++){
    readings[i] = 0;
  }
}

void InfraredSensor::init(){
  treshold = 0;
//  Serial.println("Init Infrared Sensor");
  for(int i = 0; i < 25; i++){
//    // Debug output.
//    Serial.println(getValue());
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
  
  // Calculate average over last x readings
  total -= readings[ind];
  readings[ind] = distance;
  total += readings[ind];  
  ind++;
  if (ind >= numReadings){
    ind = 0;
  }
  average = total / numReadings;

  // Deritmine if the sensor is triggered
  if (average > treshold){
    triggerCount++;
    if(triggerCount >= minimumTriggers && triggerTime + afterTriggerDelay < millis()){
//      // Debug output.
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

      float distance = 2000000*pow(average, -2.1835);
      Serial.print('A'); // Signal "Grind Activated"
      char suffix = (description == "Left")?'L':'R';
      Serial.println(suffix);
      
      // Reset some variables, set output
      triggerCount = 0;
      triggerTime = millis();
      output[0] = description;
      output[1] = (String)distance;
    }
  }else{
    triggerCount = 0;
  }
}

