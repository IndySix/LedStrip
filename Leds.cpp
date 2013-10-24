/*
  Leds.cpp - Library for flashing LedStrip code.
 Created by Michiel Dommerholt, 16 Oktober 2013.
 */

#include "Arduino.h"
#include "Leds.h"

// Constructor
Leds::Leds(const int ledsCount, const int sensorCount, const int ledsRedPins[], const int ledsGreenPins[], const int sensorPins[], const int controlInputs[], const int modeCount){
  Serial.begin(9600);
  
  
  leds = ledsCount;
  
  sensors = sensorCount;
  Serial.print("Leds: ");
  Serial.println(leds);
  
  for (int i = 0; i < ledsCount; i++){
    lR[i] = ledsRedPins[i];
    lG[i] = ledsGreenPins[i];
    
    Serial.print(i);
    Serial.print(" ");
    Serial.print(leds);
    Serial.print(" ");
    Serial.print(ledsCount);
    Serial.print(" lR[i] ");
    Serial.print(lR[i]);
    Serial.print(" lG[i] ");
    Serial.println(lG[i]);
    
    pinMode(lR[i], OUTPUT);
    pinMode(lG[i], OUTPUT);
    digitalWrite(lR[i], LOW);
    digitalWrite(lG[i], LOW);
  }
  
  
  
  ledDelay = 5 * 1000;
  mode, counter = 0;
  prevLedsStart, prevLedsEnd = -1;

  //ledOnTime[0] = {0, 0, 0, 0};

  toggle = false;
  buttonPressed = true;

  Serial.println("test1");
  analogInGrindStart = controlInputs[0];
  analogInGrindEnd = controlInputs[1];
  buttonPin = controlInputs[2];
  modes = modeCount;
  
  Serial.println("test4");
  for (int j = 0; j < sensors; j++){
    lS[j] = sensorPins[j];
    Serial.println("test4a");
    pinMode(lS[j], INPUT);
    ledStartupValue[j] = analogRead(lS[j]);
    Serial.println("test4b");
  }
  Serial.println("test5");
  pinMode(buttonPin, INPUT);
  Serial.println("test6");
  
  
  leds = ledsCount;
}

String Leds::tick() {
  int grindValueStart, grindValueEnd;

  int button = digitalRead(buttonPin);

  if(!buttonPressed && button == 1){
    buttonPressed = true;
    mode++;
    mode = (mode > modes) ? 0 : mode;
    counter = 0;
  }
  else if(button == 0){
    buttonPressed = false;
  }

  grindValueStart = analogRead(analogInGrindStart);
  grindValueEnd   = analogRead(analogInGrindEnd);

  //Serial.print("Delay: ");
  //Serial.print(grindValueStart);
  //Serial.print(" Mode:");
  //Serial.print(mode);
  //Serial.print(" ");

  switch (mode) {
  case 0:  
    return ledsLightSensors(grindValueStart);                 
    break;
  case 1:  
    return ledsOn();                                          
    break;
  case 2:  
    return ledsPotControlled(grindValueStart, grindValueEnd); 
    break;
  case 3:  
    return ledsBlink(grindValueStart);                        
    break;
  case 4:  
    return ledsWalk(grindValueStart);                         
    break;
  case 5:  
    return ledsKitt(grindValueStart);                         
    break;
  case 6:  
    return ledsBinary(grindValueStart);                       
    break;
  case 7:  
    return ledsRandom(grindValueStart);                       
    break;
  case 8:  
    return ledsToggle(grindValueStart);                       
    break;
  default: 
    return ledsOff();                                         
    break;
  }
}

String Leds::ledsLightSensors(int potMeterValue){
  //Serial.print("Light Sensor. Values: ");
  //lightSensorTreshold = potMeterValue / 5;
  for (int i = 0; i < sensors; i++)  {
    Serial.print(analogRead(lS[i]));
    Serial.print(" ");
    if(ledOnTime[i] == 0 && analogRead(lS[i]) < ledStartupValue[i] * 0.9){
      setLed(i*2, 'g');
      setLed(i*2+1, 'g');
      ledOnTime[i] = millis();
    }
    else if(ledOnTime[i] == 0){
      setLed(i*2, 'r');
      setLed(i*2+1, 'r');
    }
    if(ledOnTime[i] != 0 && ledOnTime[i] + ledDelay < millis()){
      setLed(i*2, 'r');
      setLed(i*2+1, 'r');
      ledOnTime[i] = 0;
    }
  }
  Serial.println("");
  delay(1);
  return "Light Sensor. Values: ";
}

String Leds::ledsOn(){
  //Serial.println("On");
  for (int i = 0; i < leds; i++)  {
    setLed(i, 'g');
  }
  delay(1);
  return "On";
}

String Leds::ledsPotControlled(int valStart, int valEnd){
  //Serial.println("Potmeter Controlled");
  int ledStart, ledEnd;
  ledStart = valStart/64;
  ledEnd = valEnd/64;

  if(ledStart != prevLedsStart || ledEnd != prevLedsEnd){
    for (int i = 0; i < leds; i++)  {
      setLed(i, (i < ledStart && i >= ledEnd) ? 'g' : 'r');
    }
    prevLedsStart = ledStart;
  }
  delay(1); 
  return "Potmeter Controlled";
}

String Leds::ledsBlink(int delayTime){
  //Serial.println("Blink all");
  for (int i = 0; i < leds; i++)  {
    setLed(i, (toggle) ? 'g' : 'r');
  }
  toggle = !toggle;
  delay(delayTime);
  return "Blink all";
}

String Leds::ledsWalk(int delayTime){
  //Serial.println("Walk"); 
  for (int i = 0; i < leds; i++)  {
    setLed(i, ((counter + i) % 3 == 0) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
  return "Walk";
}

String Leds::ledsKitt(int delayTime){
  //Serial.println("Kitt"); 
  float temp = millis()/1000.0;
  int ledStart = 8 + (8.0 * sin(temp * 2.0 * PI));

  for (int i = 0; i < leds; i++)  {
    setLed(i, (i != ledStart && i != ledStart -1 && i != ledStart + 1) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
  return "Kitt";
}

String Leds::ledsBinary(int delayTime){
  //Serial.print("Binary Count: ");
  //Serial.println(counter);
  for (int i = 0; i < leds; i++)  {
    int temp = pow(2, i);
    setLed(i, (counter & temp) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
  return "Binary Count: " + (String) counter;
}

String Leds::ledsRandom(int delayTime){
  //Serial.println("Random");
  for (int i = 0; i < leds; i++)  {
    setLed(i, (random(0,2) == 1) ? 'g' : 'r');
  }
  delay(delayTime);
  return "Random";
}

String Leds::ledsToggle(int delayTime){
  //Serial.println("Toggle");
  for (int i = 0; i < leds; i++)  {
    setLed(i, ((counter + i) % 2 == 0) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
  return "Toggle";
}

String Leds::ledsOff(){
  //Serial.println("Off");
  for (int i = 0; i < leds; i++)  {
    setLed(i, 'x');
  }
  delay(1);
  return "Off";
}

void Leds::setLed(int n, char value){
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


String Leds::setMode(int n){
  mode = n;
  return "Mode set to " + (String) mode;
}
