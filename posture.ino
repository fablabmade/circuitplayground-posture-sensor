#include <Adafruit_Circuit_Playground.h>

#define vibPin 9
#define motorpower 255
#define READINGS 50
#define READING_INTERVAL 100
#define THRESHOLD 1.5f

float motionAvg;
float motionReading;
float calibratedValue;
int currentReading;
int state;

void setup() {

  CircuitPlayground.begin();
  Serial.begin(9600);
  Serial.println("Circuit Playground test!");

  
  motionAvg = 0;
  currentReading = 1;
  motionReading = 0;
  state=0;
  // put your setup code here, to run once:
  pinMode(9,OUTPUT);

}

void beep(int multi){
  for(int i = 0; i<multi; i++){
    analogWrite(vibPin, motorpower); 
    delay(200);
    analogWrite(vibPin, 0); 
    delay(200);
  }
}


bool checkReading(){
    motionReading = CircuitPlayground.motionZ();  
 
    if (fabs(motionReading - calibratedValue) > THRESHOLD) {
         Serial.println(motionReading);
         return false;
    }
    return true;
}


void loop() {
  CircuitPlayground.clearPixels();

  // put your main code here, to run repeatedly:
  if (CircuitPlayground.leftButton()){
     motionAvg = 0;
  currentReading = 1;
  motionReading = 0;
  state=0;
    return;
  }
  
  if (state == 0) {
  // calibration mode
    CircuitPlayground.setPixelColor(0, 255,   0,   0);
    motionReading = CircuitPlayground.motionZ();  
    if (motionAvg == 0) {
      motionAvg = motionReading;
    }
    motionAvg = (motionAvg + motionReading ) / 2.0f;
    if (currentReading++ > READINGS) {
      calibratedValue = motionAvg;
      state = 1;
      Serial.println("Calibration complete");
      Serial.println(calibratedValue);
      return;
    }
    delay(READING_INTERVAL);
  }else
  if (state == 1) {
    // normal mode
    CircuitPlayground.setPixelColor(0, 0,  255,   0);
    
    if (!checkReading() ) {
      state = 3;
      return;
    }
    delay(READING_INTERVAL);
  } else 
  if (state == 3) {
    Serial.println("Beep");
    CircuitPlayground.setPixelColor(0, 0,  0 ,   255);
     
      beep(1);
    if (checkReading()){
      state = 1;
      return;
    }

  }
}
