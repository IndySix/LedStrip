#include "Leds.h"
#include "InfraredSensor.h"

const int   ledsCount                  = 16;                                                               // Leds
const int   ledSeparation              = 20;                                                               // Distance between leds in CM
const int   sensorCount                = 8;                                                                // Sensors
const int   ledsRedPins[ledsCount]     = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Red leds
const int   ledsGreenPins[ledsCount]   = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // Green leds
const int   lightSensors[sensorCount]  = {2, 3, 4, 5};                                                     // Light Sensors
const float sensorTreshold             = 0.6;                                                              // Sensor treshold multiplier
int         infraredMinimumTriggers    = 10;
double      infraredSensitivity        = 0.9;
int         infraredAfterTriggerDelay  = 1000;
String      descriptionLeft            = "Left";
String      descriptionRight           = "Right";

Leds ledStrip;
InfraredSensor sensorLeft( A1, descriptionLeft,  infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);
InfraredSensor sensorRight(A0, descriptionRight, infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);

boolean initialized = false;

String left[2];
String right[2];

void setup(){
  ledStrip.init(ledsCount, ledSeparation, sensorCount, ledsRedPins, ledsGreenPins, lightSensors, sensorTreshold);
  Serial.begin(9600);
}

void loop(){
  if(initialized){
    if(!ledStrip.ledsLightSensors()){
      left[0] = "null";
      left[1] = "-1";
      right[0] = "null";
      right[1] = "-1";
  
      sensorLeft.tick(left);
      sensorRight.tick(right);
      if(left[0] == descriptionLeft){
        ledStrip.startGrind(false, millis(), left[1]);
      }
      if(right[0] == descriptionRight){
        ledStrip.startGrind(true, millis(), right[1]);
      }
    }
    delay(1);
  }else{
    Serial.println("I'm the box!");
    String incomingMessage = "";
    
    while(Serial.available() > 0) {
      char incomingByte = Serial.read();
      incomingMessage += String(incomingByte);
    }
    
    if(incomingMessage == "Hi"){
//      Serial.println(" -Initializing");
      sensorLeft.init();
      sensorRight.init();
    
      ledStrip.calibrateSensors();
      Serial.println('C'); // Trigger "Calibration done" sound.
      initialized = true;
    }
    delay(50);
  }
}


