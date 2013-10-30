/*
  Leds.cpp - Library for flashing LedStrip code.
 Created by Michiel Dommerholt, 16 Oktober 2013.
 */

#include "Arduino.h"
#include "Leds.h"

// Constructor
Leds::Leds(){}

void Leds::init(const int ledsCount, const int sensorCount, const int ledsRedPins[], const int ledsGreenPins[], const int sensorPins[]){
  Serial.print(" -Leds: ");
  Serial.println(ledsCount);

  Serial.println(" -Init led pins");
  String redAddresses = " -R: {";
  String grnAddresses = " -G: {";
  for (int i = 0; i < ledsCount; i++){
    lR[i] = ledsRedPins[i];
    lG[i] = ledsGreenPins[i];
    pinMode(lR[i], OUTPUT);
    pinMode(lG[i], OUTPUT);
    digitalWrite(lR[i], LOW);
    digitalWrite(lG[i], LOW);

    setLed(i, 'r');

    redAddresses += lR[i];
    grnAddresses += lG[i];
    if(i < ledsCount - 1){
      redAddresses += ", ";
      grnAddresses += ", ";
    }
  }
  Serial.println(redAddresses + "}");
  Serial.println(grnAddresses + "}");

  Serial.println(" -Init sensor pins");
  String sensorAddresses = " -   {";
  for (int i = 0; i < sensorCount; i++){
    lS[i] = sensorPins[i];
    pinMode(lS[i], INPUT);

    sensorAddresses += lS[i];
    if(i < sensorCount - 1){
      sensorAddresses += ", ";
    }
  }
  Serial.println(sensorAddresses + "}");

  Serial.println(" -Setting default values.");
  ledDelay      = 5 * 1000;
  lastActivated = -1;
  ledOnTime     = {0, 0, 0, 0, 0, 0, 0, 0  };

  leds    = ledsCount;
  sensors = sensorCount;

  Serial.println(" -Initialization Done");
}

void Leds::ledsLightSensors(){
  //  Serial.print(" -Light Sensor. Values: ");
  //  Serial.print(activatedLed);
  //  Serial.print(" ");
  for (int i = 0; i < sensors; i++)  {
    int value = analogRead(lS[i]);
    //    Serial.print(analogRead(lS[i]));
    //    Serial.print("(");
    //    Serial.print(ledStartupValue[i]);
    //    Serial.print(") ");
    if(grinding && ledOnTime[i] == 0 && value < ledStartupValue[i] && i > lastActivated){
      setLed(i*2, 'g');
      setLed(i*2+1, 'g');
      ledOnTime[i] = millis();
      lastActivated = i;
      Serial.print("Led activated: ");
      Serial.print(i);
      Serial.print(" Value: ");
      Serial.print(value);
      Serial.print(" Tresold: ");
      Serial.println(ledStartupValue[i]);
    }
    if(ledOnTime[i] != 0 && ledOnTime[i] + ledDelay < millis()){
      setLed(i*2, 'r');
      setLed(i*2+1, 'r');
      ledOnTime[i] = 0;
      if(i == lastActivated){
        lastActivated = -1;
      }
    }
  }
  //  Serial.println("");
  delay(1);
}

void Leds::setLed(int n, char value){
  //    Serial.print("Setting led ");
  //    Serial.print(n);
  //    Serial.print(" to '");
  //    Serial.print(value);
  //    Serial.print("' on pins ");
  //    Serial.print(lG[n]);
  //    Serial.print(" & ");
  //    Serial.println(lR[n]);

  if(value == 'x'){
    digitalWrite(lG[n], LOW);
    digitalWrite(lR[n], LOW);
  }
  else if(value == 'r'){
    digitalWrite(lG[n], LOW);
    digitalWrite(lR[n], HIGH);
  }
  else if(value == 'g'){
    digitalWrite(lG[n], HIGH);
    digitalWrite(lR[n], LOW);
  }
}

void Leds::calibrateSensors(){
  Serial.println(" -Calibrating led sensors:");
  delay(500);
  for (int j = 0; j < sensors; j++){
    for (int i = 0; i < sensors; i++){
      ledStartupValue[i] += analogRead(lS[i]);
    }
  }
  Serial.print(" -");
  for (int i = 0; i < sensors; i++){
    ledStartupValue[i] = (ledStartupValue[i] / sensors) * 0.9;
    Serial.print(ledStartupValue[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void Leds::startGrind(int timestamp){
  grindStart = timeStamp;
  grinding = true;
}
