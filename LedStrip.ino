#include "Leds.h"
#include "InfraredSensor.h"

const int   ledsCount                        = 16;                                                               // Leds
const int   ledSeparation                    = 10;                                                               // Distance between leds in CM
const int   sensorCount                      = 20;                                                               // Sensors
const int   multiplexChips                   = 3;
const int   clockpins[multiplexChips]        = {13, 9, 5};
const int   misopins[multiplexChips]         = {12, 8, 4};
const int   mosipins[multiplexChips]         = {11, 7, 3};
const int   cspins[multiplexChips]           = {10, 6, 53};
const float sensorTreshold                   = 0.8;                                                              // Sensor treshold multiplier
int         infraredMinimumTriggers          = 10;
double      infraredSensitivity              = 0.9;
int         infraredAfterTriggerDelay        = 1000;
String      descriptionLeft                  = "Left";
String      descriptionRight                 = "Right";

Leds ledStrip;
const int ledCountTop    = 118;
const int ledCountBottom = 114;

//InfraredSensor sensorLeft( A1, descriptionLeft,  infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);
//InfraredSensor sensorRight(A0, descriptionRight, infraredMinimumTriggers, infraredAfterTriggerDelay, infraredSensitivity);

boolean initialized = false;

String left[2];
String right[2];

void setup(){
  for(int i = 0; i < multiplexChips; i++){
    pinMode(clockpins[i], OUTPUT);
    pinMode(mosipins[i], OUTPUT);
    pinMode(misopins[i], INPUT);
    pinMode(cspins[i], OUTPUT);
  }
  
  ledStrip.init(ledCountTop,
                ledCountBottom,
                ledSeparation,
                sensorCount,
                multiplexChips,
                clockpins,
                mosipins,
                misopins,
                cspins,
                sensorTreshold);
  Serial.begin(9600);
}

void loop(){
//   ledsKitt(1);
   
  if(initialized){
  /*  if(!*/ledStrip.ledsLightSensors();//){
//      left[0] = "null";
//      left[1] = "-1";
//      right[0] = "null";
//      right[1] = "-1";
//  
//      sensorLeft.tick(left);
//      sensorRight.tick(right);
//      if(left[0] == descriptionLeft){
//        ledStrip.startGrind(false, millis(), left[1]);
//      }
//      if(right[0] == descriptionRight){
//        ledStrip.startGrind(true, millis(), right[1]);
//      }
//    }
//    delay(1);
  }else{
//    Serial.println("I'm the box!");
//    String incomingMessage = "";
//    
//    while(Serial.available() > 0) {
//      char incomingByte = Serial.read();
//      incomingMessage += String(incomingByte);
//    }
//    
//    if(incomingMessage == "Hi"){
//      Serial.println(" -Initializing");
 //     sensorLeft.init();
 //     sensorRight.init();
 //   
      ledStrip.calibrateSensors();
      Serial.println('C'); // Trigger "Calibration done" sound.
      initialized = true;
//    }
//    delay(50);
  }
}

/*
void ledsKitt(int delayTime){
  Serial.println("Kitt");
  float temp = millis()/1000.0;
  int posTop    = 59 + (59.0 * sin(temp * 2.0 * PI)); // 118
  int posBottom = 57 + (57.0 * sin(temp * 2.0 * PI)); // 114
  
  for (int i = 0; i < ledCountTop; i++){
    if((i > posTop - 5) && (i < posTop + 5)){
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }else{
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
  }
  
  for (int i = 0; i < ledCountBottom; i++){
    if((i > posBottom - 5) && (i < posBottom + 5)){
      strip.setPixelColor(strip.numPixels()-(i+1), strip.Color(255, 0, 0));
    }else{
      strip.setPixelColor(strip.numPixels()-(i+1), strip.Color(0, 255, 0));
    }
  }
  strip.show();
  
  delay(delayTime);
}*/


