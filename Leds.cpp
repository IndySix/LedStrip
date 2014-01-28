/*
  Leds.cpp - Library for flashing LedStrip code.
  Created by Michiel Dommerholt, 16 Oktober 2013.
*/

#include "Arduino.h"
#include "Leds.h"
#include "ADConverter.h"
//#include <Wire.h>
#include "Adafruit_NeoPixel.h"

// Ledstrip data pin
#define PIN 2

// Declare a "strip" variable. Params: # of leds, data pin.
Adafruit_NeoPixel strip  = Adafruit_NeoPixel(117, PIN, NEO_GRB + NEO_KHZ800);

//// Define a few colours
uint32_t red   = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue  = strip.Color(0, 0, 255);
uint32_t off   = strip.Color(0, 0, 0);
uint32_t white = strip.Color(255, 255, 255);

// Constructor
Leds::Leds(){
//  Wire.begin();
}

// Initialize the strip, pass along arrays of pin addresses etc.
void Leds::init(const int ledCount,
                const int ledSeparation,
                const int sensorCount,
                const int multiplexChips,
                const int clockpin,
                const int mosipin,
                const int misopin,
                const int cspins[],
                const float sensorTreshold){
  // "Start" the ledstrip
  strip.begin();
  
  // Set all leds to red.
  for (int i = 0; i < ledCount; i++){
    strip.setPixelColor(i, red);
  }
  
  // "Update" the strip
  strip.show();
  
//  Serial.print(" -Leds: ");
//  Serial.println(ledCount);

  // Copy multiplex chip addresses
  for(int i = 0; i < multiplexChips; i++){
    cs[i]    = cspins[i];
  }
  clock = clockpin;
  mosi  = mosipin;
  miso  = misopin;

//  Serial.println(" -Setting default values.");
  ledDelay      = 5 * 1000; // Time in ms it takes before a activated led's switched off.
  lastActivated = -1;       // Last activated sensor ID.

  leds               = ledCount;
  sensors            = sensorCount;
  ledDistance        = ledSeparation;
  tresholdMultiplier = sensorTreshold;
  grinding           = false;
  infraredWorkaround = false;
  
  // Array to save activation timestamps of sensors
  for(int i = 0; i < sensors; i++){
    ledOnTime[i] = -1;
  }

//  Serial.println(" -Initialization Done");
}

boolean Leds::ledsLightSensors(){
//  // Debug output
//  Serial.print(" -Light Sensor. Values: ");
//  String output = "";

  for (int i = 0; i < sensors; i++)  {
    int value = getSensorValue(i);
    
//    // Debug output
//    output += (String)value;
//    if(value < 10){
//      output+=" ";
//    }
//    output += ":" + (String)ledStartupValue[i] + "." + (String)ledOnTime[i] + ", ";
    

    if( !grinding &&
        value < ledStartupValue[i]){
      String temp = "0.0";
      startGrind(false, millis(), temp);
      infraredWorkaround = true;
      Serial.println("AR");
    }

    // If the strip was activated by a infrared sensor &&
    //    the currently active sensor hasn't been activated yet &&
    //    the currently active sensor follows the right direction || no sensors are activated yet.
    if( grinding &&
        ledOnTime[i] == -1 &&
        value < ledStartupValue[i] &&
        ((grindDirection == i > lastActivated) ||
         lastActivated == -1 ||
         infraredWorkaround)
      ){
        
      // Get the grind direction
      if(infraredWorkaround && lastActivated != -1){
        grindDirection = (lastActivated < i);
        infraredWorkaround = false;
      }
        
      // Save the activation time of this sensor
      ledOnTime[i] = millis();
      
      // save what sensor ID was last activated.
      lastActivated = i;
      lastActivatedTime = millis();
      
      // Debug output
      Serial.print("Led activated: ");
      Serial.print(i);
      Serial.print(" Value: ");
      Serial.print(value);
      Serial.print(" Tresold: ");
      Serial.print(ledStartupValue[i]);
      Serial.print(" Time: ");
      Serial.print(ledOnTime[i]);
      Serial.print(" Time: ");
      Serial.println(millis());
      
//      // Trigger "Led enabled" sound.
//      Serial.print('T');
//      if(i < 10){
//        Serial.print('0');
//      }
//      Serial.println(i);

      sensorActivated(i, green);
    }
    
    // If the current sensor has been activated &&
    //    the activation was longer than ledDelay ago.
    if(lastActivatedTime + ledDelay < millis() && i == lastActivated){
      buildOutput();
    }
  }
  
//  // Update the strip.
//  strip.show();
  
//  // Debug output.
//  Serial.println(output);
  
  return grinding;
}

void Leds::buildOutput(){
  // Reset lastActivated
  lastActivated = -1;

  // Initialize some variables
  int firstLed = -1;
  int lastLed = -1;
  int firstLedTime = -1;
  int firstActivatedLedTime = 999999999;
  int lastLedTime = -1;
  String jsonOut = "F{";

  // Build the array with sensor activation times.
  String ledTimes = "[";
  // Loop through all sensors
  for (int j = 0; j < sensors; j++){
    // Set the leds back to red.
    sensorActivated(j, red);
    
    // Substract the grind start time from the activation time, to get the time since the grind started.
    ledTimes += (ledOnTime[j] == -1 ? "-1" : (String)(ledOnTime[j] - grindStartTime));
    if(ledOnTime[j] != -1){
      // Find the first activated led & time.
      if(firstLed == -1){
        firstLed = j;
        firstLedTime = ledOnTime[j];
      }
      // Find the last activated led & time.
      lastLed = j;
      lastLedTime = ledOnTime[j];
      if(firstActivatedLedTime > ledOnTime[j]){
        firstActivatedLedTime = ledOnTime[j];
      }
    }
    // Reset the activation time for this sensor.
    ledOnTime[j] = -1;
    if(j < sensors - 1){
      ledTimes += ", ";
    }
  }
  // Finish the array string. Format: "[123,128,332,756,-1,-1](etc)"
  ledTimes += "]";
  
  int distance = (lastLed - firstLed) * ledDistance;                              // Calculate the grind distance.
  int duration = abs(lastLedTime - firstLedTime);                                 // Calculate the grind duration
  float percentage = ((float)(lastLed - firstLed + 1) / (float) sensors) * 100.0; // Calculate the grind percentage.
  
  // Convert the grindStartDistance String to a float
  char floatbuf[32];
  grindStartDistance.toCharArray(floatbuf, sizeof(floatbuf));
  float sensorDistance = atof(floatbuf);
  
  float km = sensorDistance / 1000.0;                              // Calculate the grind distance.
  float hours = ((float)firstActivatedLedTime)/(1000.0*60.0*60.0); // Calculate the grind duration.
  float velocity = km/hours;                                       // Calculate the grind velocity.
  
  // Combine the data into a json string
  jsonOut +=
      "\"leds\":"           +          ledTimes                                    +
    ", \"distance\":"       + (String) distance                                    +
    ", \"duration\":"       + (String) duration                                    +
    ", \"velocity\":"       + (String) velocity                                    +
    ", \"percentage\":"     + (String) percentage                                  +
    ", \"direction\":"      + (String) (grindDirection ? "\"Right\"" : "\"Left\"") +
    ", \"startDistance\":"  + (String) grindStartDistance                          ;
  
  // Output the grind data.
  Serial.println(jsonOut + "}");
  // End Output
  
  // Disable the grind.
  grinding = false; 
}

void Leds::setLed(int n, uint32_t c){
  // Set led n to a colour.
  strip.setPixelColor(n, c);
}

void Leds::sensorActivated(int n, uint32_t c){
  if(n == 19){
    sensorActivated(18, c);
  }
  float sectionPixels = (double)leds / (double)sensors;
  for(int i = sectionPixels * n; i < sectionPixels * (n+1); i++){
    strip.setPixelColor(leds - (i + 1), c);
  }
  //Serial.println(grindDirection);
  strip.show();
}

void Leds::calibrateSensors(){
//  // Debug output.
//  Serial.println(" -Calibrating led sensors:");
  // Wait half a second.
  delay(500);
  int counter = 0;
  
  for (int j = 0; j < sensors; j++){
    for (int i = 0; i < sensors; i++){
      ledStartupValue[i] += getSensorValue(i);
    }
    for (int i = 0; i < leds; i++)  {
      setLed(i, ((counter - i) % 3 == 0) ? green : red);
    }
    strip.show();
    counter++;
    delay(100);
  }
//  // Debug output
//  Serial.print(" -");
  for (int i = 0; i < sensors; i++){
    ledStartupValue[i] = (ledStartupValue[i] / sensors) * tresholdMultiplier;
//    // Debug output
//    Serial.print(ledStartupValue[i]);
//    Serial.print(" ");
  }

  // "Calibration ready" flashing
  for (int j = 0; j < sensors; j++)  {
    for (int i = 0; i < leds; i++)  {
      setLed(i, (j % 2 == 0) ? green : red);
    }
    strip.show();
    delay(50);
  }
  
//  // Debug output
//  Serial.println("");
}

void Leds::startGrind(boolean grindPositive, int startTime, String& startDistance){
  // If the box isn't currently "active", initialize / copy some variables.
  if(!grinding){
    grinding           = true;
    grindDirection     = grindPositive;
    grindStartTime     = startTime;
    grindStartDistance = (String)startDistance;
  }
//  Serial.println(
//    (String)grinding + ", " +
//    (String)grindDirection + ", " +
//    (String)grindStartTime + ", " +
//    (String)grindStartDistance + ";");
}

int Leds::getSensorValue(int sensorID){
  // Calculate chip number and sensor number on that chip, from the sensor ID.
  if(sensorID == 18){
     return 999; 
  }
  
  int chipNo = (int)(sensorID / 8);
  int sensorAddress = sensorID % 8;
  
  // Read the chip at the specified address.
  ADConverter adc;
  return adc.readAdc(sensorAddress, clock, mosi, miso, cs[chipNo]);
}

