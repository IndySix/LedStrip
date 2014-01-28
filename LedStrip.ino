#include "Leds.h"
//#include "InfraredSensor.h"

const float sensorTreshold            = 0.4; // Sensor treshold multiplier (Sensitivity) Lower is less sensitive.

const int   ledCount                  = 117; // Leds
const int   ledSeparation             = 10;  // Distance between leds in CM
const int   sensorCount               = 20;  // Sensors
const int   multiplexChips            = 3;
const int   clockpin                  = 13;
const int   misopin                   = 12;
const int   mosipin                   = 11;
const int   cspins[multiplexChips]    = {10, 9, 8}; // {10, 6, 2};
int         infraredMinimumTriggers   = 10;
double      infraredSensitivity       = 0.8;
int         infraredAfterTriggerDelay = 1000;
String      descriptionLeft           = "Left";
String      descriptionRight          = "Right";

Leds ledStrip;

//InfraredSensor sensorLeft( A1, descriptionLeft,  infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);
//InfraredSensor sensorRight(A0, descriptionRight, infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);

boolean initialized = false;

String left[2];
String right[2];

void setup(){
  Serial.begin(9600);
//  Serial.println(" -Setup");
  for(int i = 0; i < multiplexChips; i++){
    pinMode(cspins[i], OUTPUT);
  }
  pinMode(clockpin, OUTPUT);
  pinMode(mosipin, OUTPUT);
  pinMode(misopin, INPUT);
  
  ledStrip.init(ledCount, ledSeparation, sensorCount, multiplexChips, clockpin, mosipin, misopin, cspins, sensorTreshold);
}

void loop(){
  if(initialized){
    ledStrip.ledsLightSensors();
  }else{
      ledStrip.calibrateSensors();
      Serial.println('C'); // Trigger "Calibration done" sound.
      initialized = true;
  }
}

//// Old code including the infrared sensors.
//if(!ledStrip.ledsLightSensors()){
//  left[0] = "null";
//  left[1] = "-1";
//  right[0] = "null";
//  right[1] = "-1";
//
//  sensorLeft.tick(left);
//  sensorRight.tick(right);
//
//  Serial.print(left[0]);
//  Serial.print(' ');
//  Serial.print(left[1]);
//  Serial.print(' ');
//  Serial.print(right[0]);
//  Serial.print(' ');
//  Serial.println(right[1]);
//  if(left[0] == descriptionLeft){
//    ledStrip.startGrind(false, millis(), left[1]);
//  }
//  if(right[0] == descriptionRight){
//    ledStrip.startGrind(true, millis(), right[1]);
//  }
//}
//delay(1);
