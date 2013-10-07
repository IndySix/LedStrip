const int lG[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // Green leds
const int lR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Red leds
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
  
  /*for (int i = 0; i < sensors; i++){
    pinMode(lS[i], INPUT);
  }*/
  
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
 mode = 0; 
}

void loop() {
  int button = digitalRead(buttonPin);
  if(!buttonPressed && button == 1){
    buttonPressed = true;
    mode++;
    mode = (mode > modes) ? 0 : mode;
    counter = 0;
  }else if(button == 0){
    buttonPressed = false;
  }
  
  int grindValueStart, grindValueEnd, ledStart, ledEnd;
  float something;
  
  grindValueStart = analogRead(analogInGrindStart);
  grindValueEnd = analogRead(analogInGrindEnd);
  
  Serial.print("Delay: ");
  Serial.print(grindValueStart);
  Serial.print(" Mode:");
  Serial.print(mode);
  Serial.print(" ");
  
  switch (mode) {
    case 0:
      Serial.print("Light Sensor. Values: ");
      lightSensorTreshold = grindValueStart / 5;
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
      break;
    case 1:
      Serial.println("On");
      for (int i = 0; i < leds; i++)  {
        setLed(i, 'g');
      }
      delay(1);
      break;
    case 2:
      Serial.println("Default operation");
      ledStart = grindValueStart/64;
      ledEnd = grindValueEnd/64;
      
      if(ledStart != prevLedsStart || ledEnd != prevLedsEnd){
        for (int i = 0; i < leds; i++)  {
          setLed(i, (i < ledStart && i >= ledEnd) ? 'g' : 'r');
        }
        prevLedsStart = ledStart;
      }
      delay(1);
      break;
    case 3:
      Serial.println("Toggle all");
      for (int i = 0; i < leds; i++)  {
        setLed(i, (toggle) ? 'g' : 'r');
      }
      toggle = !toggle;
      delay(grindValueStart);
      break;
    case 4:
      Serial.println("Walk"); 
      for (int i = 0; i < leds; i++)  {
        setLed(i, ((counter + i) % 3 == 0) ? 'g' : 'r');
      }
      counter++;
      delay(grindValueStart);
      break;
    case 5:
      Serial.println("Kitt"); 
      something = millis()/1000.0;
      ledStart = 8 + (8.0 * sin(something * 2.0 * PI));
      
      for (int i = 0; i < leds; i++)  {
        setLed(i, (i != ledStart && i != ledStart -1 && i != ledStart + 1) ? 'g' : 'r');
      }
      counter++;
      delay(grindValueStart);
    
      break;
    case 6:
      Serial.print("Binary Count: ");
      Serial.println(counter);
      for (int i = 0; i < leds; i++)  {
        int temp = pow(2, i);
        setLed(i, (counter & temp) ? 'g' : 'r');
      }
      counter++;
      delay(grindValueStart);
      break;
    case 7:
      Serial.println("Random");
      for (int i = 0; i < leds; i++)  {
        setLed(i, (random(0,2) == 1) ? 'g' : 'r');
      }
      delay(grindValueStart);
      break;
    case 8:
      Serial.println("Toggle");
      for (int i = 0; i < leds; i++)  {
        setLed(i, ((counter + i) % 2 == 0) ? 'g' : 'r');
      }
      counter++;
      delay(grindValueStart);
      break;
    default:
      Serial.println("Off");
      for (int i = 0; i < leds; i++)  {
        setLed(i, 'x');
      }
      break;
  }
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
