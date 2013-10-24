#include "Leds.h"

const int ledsCount       = 16;                                                               // Leds
const int sensorCount     = 8;                                                                // Sensors
const int ledsRedPins[]   = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Red leds
const int ledsGreenPins[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // Green leds
const int lightSensors[]  = {A7, A6, A5, A4, A3, A2, A1, A0};                                 // Light Sensors
const int inputs[]        = {A10, A11, 10};                                                   // Inputs: Pot 1, Pot 2, Button.
const int modeCount       = 9;                                                                // Modes


void setup(){
  Leds ledStrip(ledsCount,     //int ledsCount
                sensorCount,   //int sensorCount
                ledsRedPins,   //int ledsRedPins[]
                ledsGreenPins, //int ledsGreenPins[]
                lightSensors,  //int sensorPins[]
                inputs,        //int controlInputs[] (start, end, button)
                modeCount);    //int modeCount 

 /* Serial.begin(9600);
  delay(2500);
  Serial.println("");*/
  Serial.println(ledStrip.ledsOn());
}

void loop(){
  //delay(100);
  //ledStrip.ledsOn();
}


/*const int lR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Red leds
const int lG[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // Green leds
const int lS[] = {A7, A6, A5, A4, A3, A2, A1, A0}; // Light Sensors
const int analogInGrindStart = A10;
const int analogInGrindEnd = A11;
const int buttonPin = 10;
const int modes = 9;
const int leds = 16;
const int sensors = 8;

int lightSensorTreshold = 90;
const int ledDelay = 5 * 1000; // Seconds the leds stay on after being activated by the light sensor

int mode = 0;
int counter = 0;
int prevLedsStart, prevLedsEnd = -1;
int ledOnTime[] = {0, 0, 0, 0};
boolean toggle = false;
boolean buttonPressed = true;

void setup(){
  for (int i = 0; i < leds; i++)  {
    pinMode(lG[i], OUTPUT);
    pinMode(lR[i], OUTPUT);
    digitalWrite(lG[i], LOW);
    digitalWrite(lR[i], LOW);
  }
  
  //for (int i = 0; i < sensors; i++){
  //  pinMode(lS[i], INPUT);
  //}
  
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
 mode = 0; 
}

void loop() {
  int grindValueStart, grindValueEnd;
  
  int button = digitalRead(buttonPin);
  
  if(!buttonPressed && button == 1){
    buttonPressed = true;
    mode++;
    mode = (mode > modes) ? 0 : mode;
    counter = 0;
  }else if(button == 0){
    buttonPressed = false;
  }
  
  grindValueStart = analogRead(analogInGrindStart);
  grindValueEnd   = analogRead(analogInGrindEnd);
  
  Serial.print("Delay: ");
  Serial.print(grindValueStart);
  Serial.print(" Mode:");
  Serial.print(mode);
  Serial.print(" ");
  
  switch (mode) {
    case 0:  ledsLightSensors(grindValueStart);                 break;
    case 1:  ledsOn();                                          break;
    case 2:  ledsPotControlled(grindValueStart, grindValueEnd); break;
    case 3:  ledsBlink(grindValueStart);                        break;
    case 4:  ledsWalk(grindValueStart);                         break;
    case 5:  ledsKitt(grindValueStart);                         break;
    case 6:  ledsBinary(grindValueStart);                       break;
    case 7:  ledsRandom(grindValueStart);                       break;
    case 8:  ledsToggle(grindValueStart);                       break;
    default: ledsOff();                                         break;
  }
}

void ledsLightSensors(int potMeterValue){
    Serial.print("Light Sensor. Values: ");
    lightSensorTreshold = potMeterValue / 5;
    for (int i = 0; i < sensors; i++)  {
      Serial.print(analogRead(lS[i]));
      Serial.print(" ");
      if(ledOnTime[i] == 0 && analogRead(lS[i]) < lightSensorTreshold){
        setLed(i*2, 'g');
        setLed(i*2+1, 'g');
        ledOnTime[i] = millis();
      }else if(ledOnTime[i] == 0){
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
}

void ledsOn(){
  Serial.println("On");
  for (int i = 0; i < leds; i++)  {
    setLed(i, 'g');
  }
  delay(1);
}

void ledsPotControlled(int valStart, int valEnd){
    Serial.println("Potmeter Controlled");
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
}

void ledsBlink(int delayTime){
  Serial.println("Blink all");
  for (int i = 0; i < leds; i++)  {
    setLed(i, (toggle) ? 'g' : 'r');
  }
  toggle = !toggle;
  delay(delayTime);
}

void ledsWalk(int delayTime){
  Serial.println("Walk"); 
  for (int i = 0; i < leds; i++)  {
    setLed(i, ((counter + i) % 3 == 0) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
}

void ledsKitt(int delayTime){
  Serial.println("Kitt"); 
  float temp = millis()/1000.0;
  int ledStart = 8 + (8.0 * sin(temp * 2.0 * PI));
  
  for (int i = 0; i < leds; i++)  {
    setLed(i, (i != ledStart && i != ledStart -1 && i != ledStart + 1) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
}

void ledsBinary(int delayTime){
  Serial.print("Binary Count: ");
  Serial.println(counter);
  for (int i = 0; i < leds; i++)  {
    int temp = pow(2, i);
    setLed(i, (counter & temp) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
}

void ledsRandom(int delayTime){
  Serial.println("Random");
  for (int i = 0; i < leds; i++)  {
    setLed(i, (random(0,2) == 1) ? 'g' : 'r');
  }
  delay(delayTime);
}

void ledsToggle(int delayTime){
  Serial.println("Toggle");
  for (int i = 0; i < leds; i++)  {
    setLed(i, ((counter + i) % 2 == 0) ? 'g' : 'r');
  }
  counter++;
  delay(delayTime);
}

void ledsOff(){
  Serial.println("Off");
  for (int i = 0; i < leds; i++)  {
    setLed(i, 'x');
  }
  delay(1);
}

void setLed(int n, char value){
  if(value == 'x'){
      digitalWrite(lG[n], LOW);
      digitalWrite(lR[n], LOW);
  }else if(value == 'r'){
      digitalWrite(lG[n], LOW);
      digitalWrite(lR[n], HIGH);
  }else if(value == 'g'){
      digitalWrite(lG[n], HIGH);
      digitalWrite(lR[n], LOW);
  }
}
*/
