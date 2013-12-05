/*
  Leds.cpp - Library for flashing LedStrip code.
  Created by Michiel Dommerholt, 16 Oktober 2013.
*/

#include "Arduino.h"
#include "Leds.h"
#include "ADConverter.h"

ADConverter adc;

// Constructor
Leds::Leds(int clock, int dataOut, int dataIn, int chipSelect){
  SPICLK = clock;     // Orange
  SPIMISO = dataOut;  // Yellow
  SPIMOSI = dataIn;   // Blue
  SPICS = chipSelect; // Brown
}

void Leds::init(const int ledsCount, const int ledSeparation, const int sensorCount, const int ledsRedPins[], const int ledsGreenPins[], const int sensorPins[], const float sensorTreshold){
//  Serial.print(" -Leds: ");
//  Serial.println(ledsCount);
//
//  Serial.println(" -Init led pins");
//  String redAddresses = " -R: {";
//  String grnAddresses = " -G: {";
  for (int i = 0; i < ledsCount; i++){
    lR[i] = ledsRedPins[i];
    lG[i] = ledsGreenPins[i];
    pinMode(lR[i], OUTPUT);
    pinMode(lG[i], OUTPUT);
    digitalWrite(lR[i], LOW);
    digitalWrite(lG[i], LOW);

    setLed(i, 'r');

//    redAddresses += lR[i];
//    grnAddresses += lG[i];
//    if(i < ledsCount - 1){
//      redAddresses += ", ";
//      grnAddresses += ", ";
//    }
  }
//  Serial.println(redAddresses + "}");
//  Serial.println(grnAddresses + "}");
//
//  Serial.println(" -Init sensor pins");
//  String sensorAddresses = " -   {";
  for (int i = 0; i < sensorCount; i++){
    lS[i] = sensorPins[i];
//    pinMode(lS[i], INPUT);
//
//    sensorAddresses += lS[i];
//    if(i < sensorCount - 1){
//      sensorAddresses += ", ";
//    }
  }
//  Serial.println(sensorAddresses + "}");

  pinMode(SPIMOSI, OUTPUT);
  pinMode(SPIMISO, INPUT);
  pinMode(SPICLK, OUTPUT);
  pinMode(SPICS, OUTPUT);

//  Serial.println(" -Setting default values.");
  ledDelay      = 5 * 1000;
  lastActivated = -1;
  ledOnTime     = {
    -1, -1, -1, -1, -1, -1, -1, -1  };

  leds               = ledsCount;
  sensors            = sensorCount;
  ledDistance        = ledSeparation;
  tresholdMultiplier = sensorTreshold;

//  Serial.println(" -Initialization Done");
}

boolean Leds::ledsLightSensors(){
//  Serial.print(" -Light Sensor. Values: ");
//  Serial.print(activatedLed);
//  Serial.print(" ");
  for (int i = 0; i < sensors; i++)  {
    int value = getSensorValue(i);
//    Serial.print(getSensorValue(i));
//    Serial.print("(");
//    Serial.print(ledStartupValue[i]);
//    Serial.print(") ");
    if( grinding &&
        ledOnTime[i] == -1 &&
        value < ledStartupValue[i] &&
        ((grindDirection == i > lastActivated) ||
         lastActivated == -1)
      ){
      setLed(i*2, 'g');
      setLed(i*2+1, 'g');
      ledOnTime[i] = millis();
      lastActivated = i;
//      Serial.print("Led activated: ");
//      Serial.print(i);
//      Serial.print(" Value: ");
//      Serial.print(value);
//      Serial.print(" Tresold: ");
//      Serial.println(ledStartupValue[i]);
      Serial.println('T'); // Trigger "Led enabled" sound.
    }
    if(ledOnTime[i] != -1 && ledOnTime[i] + ledDelay < millis()){
      setLed(i*2, 'r');
      setLed(i*2+1, 'r');
      if(i == lastActivated){
        lastActivated = -1;

        // Output
        int firstLed = -1;
        int lastLed = -1;
        int firstLedTime = -1;
        int lastLedTime = -1;
        String jsonOut = "F{";

        String ledTimes = "[";
        for (int j = 0; j < sensors; j++){
          ledTimes += (ledOnTime[j] == -1 ? "-1" : (String)(ledOnTime[j] - grindStartTime));
          if(ledOnTime[j] != -1){
            if(firstLed == -1){
              firstLed = j;
              firstLedTime = ledOnTime[j];
            }
            lastLed = j;
            lastLedTime = ledOnTime[j];
          }

          ledOnTime[j] = -1;
          if(j < sensors - 1){
            ledTimes += ", ";
          }
        }
        ledTimes += "]";

        int distance = (lastLed - firstLed) * ledDistance;
        int duration = abs(lastLedTime - firstLedTime);
        float percentage = ((float)(lastLed - firstLed + 1) / (float) sensors) * 100.0;

        jsonOut +=
           "\"leds\":"            +          ledTimes                            +
          ", \"distance\":"       + (String) distance                            +
          ", \"duration\":"       + (String) duration                            +
          ", \"percentage\":"     + (String) percentage                          +
          ", \"direction\":"      + (String) (grindDirection ? "Right" : "Left") +
          ", \"startDistance\":"  + (String) grindStartDistance                  ;
          
        Serial.println(jsonOut + "}");
        // End Output
        
        grinding = false;
      }
    }
  }
//  Serial.println("");
  return grinding;
  delay(1);
}

void Leds::setLed(int n, char value){
//    Serial.println("Setting led " + (String) n " to '" (String) value "' on pins " (String) lG[n] " & " (String) lR[n]);
  switch(value){
  case 'x':
    digitalWrite(lG[n], LOW);
    digitalWrite(lR[n], LOW);
    break;
  case 'r':
    digitalWrite(lG[n], LOW);
    digitalWrite(lR[n], HIGH);
    break;
  case 'g':
    digitalWrite(lG[n], HIGH);
    digitalWrite(lR[n], LOW);
    break;
  }
}

void Leds::calibrateSensors(){
//  Serial.println(" -Calibrating led sensors:");
  delay(500);
  int counter = 0;
  for (int j = 0; j < sensors; j++){
    delay(500);
    for (int i = 0; i < sensors; i++){
      ledStartupValue[i] += getSensorValue(i);
    }
    for (int i = 0; i < leds; i++)  {
      setLed(i, ((counter + i) % 3 == 0) ? 'g' : 'r');
    }
    counter++;
  }
//  Serial.print(" -");
  for (int i = 0; i < sensors; i++){
    ledStartupValue[i] = (ledStartupValue[i] / sensors) * tresholdMultiplier;
//    Serial.print(ledStartupValue[i]);
//    Serial.print(" ");
  }

  for (int j = 0; j < leds; j++)  {
    for (int i = 0; i < leds; i++)  {
      setLed(i, (j % 2 == 0) ? 'g' : 'r');
    }
    delay(50);
  }
//  Serial.println("");
}

void Leds::startGrind(boolean grindPositive, int startTime, String& startDistance){
  if(!grinding){
    grinding           = true;
    grindDirection     = grindPositive;
    grindStartTime     = startTime;
    grindStartDistance = (String)startDistance;
  }
}

int Leds::getSensorValue(int sensorID){
  return adc.readAdc(sensorID, SPICLK, SPIMOSI, SPIMISO, SPICS);
}

