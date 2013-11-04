#include "Leds.h"

const int   ledsCount                  = 16;                                                               // Leds
const int   ledSeparation              = 20;                                                               // Distance between leds in CM
const int   sensorCount                = 8;                                                                // Sensors
const int   ledsRedPins[ledsCount]     = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Red leds
const int   ledsGreenPins[ledsCount]   = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // Green leds
const int   lightSensors[sensorCount]  = {A7, A6, A5, A4, A3, A2, A1, A0};                                 // Light Sensors
const float sensorTreshold             = 0.8;                                                              // Sensor treshold multiplier

Leds ledStrip;

int counter = 0;

void setup(){
  Serial.begin(9600);
//  Serial.println(" -Initializing");
  
  ledStrip.init(ledsCount, ledSeparation, sensorCount, ledsRedPins, ledsGreenPins, lightSensors, sensorTreshold);
  ledStrip.calibrateSensors();
  ledStrip.startGrind(1337);
}

void loop(){
  ledStrip.ledsLightSensors();
}
