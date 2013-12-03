#include "Leds.h"
#include "InfraredSensor.h"

const int   ledsCount                  = 16;                                                               // Leds
const int   ledSeparation              = 20;                                                               // Distance between leds in CM
const int   sensorCount                = 8;                                                                // Sensors
const int   ledsRedPins[ledsCount]     = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Red leds
const int   ledsGreenPins[ledsCount]   = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // Green leds
const int   lightSensors[sensorCount]  = {0, 1, 2, 3, 4, 5, 6, 7};//{A7, A6, A5, A4, A3, A2, A1, A0};                                 // Light Sensors
const float sensorTreshold             = 0.9;                                                              // Sensor treshold multiplier
int         infraredMinimumTriggers    = 10;
double      infraredSensitivity        = 0.9;
int         infraredAfterTriggerDelay  = 1000;

Leds ledStrip(2, 3, 4, 5);
InfraredSensor sensorLeft( A1, "Left",  infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);
InfraredSensor sensorRight(A0, "Right", infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);

int counter = 0;

void setup(){
  Serial.begin(9600);
//  Serial.println(" -Initializing");
  
  sensorLeft.init();
  sensorRight.init();

  ledStrip.init(ledsCount, ledSeparation, sensorCount, ledsRedPins, ledsGreenPins, lightSensors, sensorTreshold);
  ledStrip.calibrateSensors();
  ledStrip.startGrind(1337);
}

void loop(){
  ledStrip.ledsLightSensors();
//  Serial.print(sensorLeft.tick());
//  Serial.print(" : ");
//  Serial.println(sensorRight.tick());
  sensorLeft.tick();
  sensorRight.tick();
  delay(1);
}
