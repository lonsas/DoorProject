#include "Arduino.h"
int sensorPin = A9;
int ledPin = 13;
int sensorValue = 0;
int sensorValue2 = 0;
int thres = 1000;
bool passed = false;
bool prevStateOn = false;
bool play_pressed = false;
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
}

void loop() {
  while(Serial1.available()) {
    Serial.write(Serial1.read());
  }
  sensorValue = analogRead(sensorPin);
  if(sensorValue < thres) {
    prevStateOn = true;
  } else if(prevStateOn) {
    prevStateOn=false;
    delay(1000);
    
    passed = !passed;

  }
  if(passed) {
    digitalWrite(ledPin, HIGH);
    Serial1.write("AT+CIPSTART=\"TCP\",\"31.208.43.115\",8000\r\n");
    delay(500);
    Serial1.write("AT+CIPSEND=8\r\n");
    delay(100);
    Serial1.write("DOOROPEN");
    Serial1.write("AT+CIPCLOSE\r\n");
    passed = false;

  }
    Serial.println(passed);
    Serial.println(thres);
    Serial.println(sensorValue);
    Serial.println();
}

