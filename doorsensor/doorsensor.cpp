#include "Arduino.h"

#define DEBUG

int sensorPin = A9;
int espEnPin = 3;
int ledPin = 2;
int sensorValue = 0;
int thres = 1000;
bool doorOpen = false;
bool prevDoorOpen = false;
bool in = true;
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(espEnPin, OUTPUT);
  digitalWrite(espEnPin, HIGH);
}

void sendCommand() {
    digitalWrite(espEnPin, HIGH);
    delay(2000);
    do {
        Serial1.write("AT+CIPSTART=\"TCP\",\"31.208.43.115\",8000\r\n");
        delay(500);
        Serial1.write("AT+CIPSEND=8\r\n");
        delay(100);
        Serial1.write("DOOROPEN");
        Serial1.write("AT+CIPCLOSE\r\n");
        delay(500);
    } while(!Serial1.find("CONNECT"));
    digitalWrite(espEnPin, LOW);
}
void loop() {
#ifdef DEBUG
    while(Serial1.available()) {
        Serial.write(Serial1.read());
    }
#endif
    digitalWrite(ledPin, HIGH);
    // Give the led time to shine, doesn't work otherwise
    delay(5);
    sensorValue = analogRead(sensorPin);
    digitalWrite(ledPin, LOW);
    doorOpen = sensorValue > thres;
    if(prevDoorOpen && !doorOpen) {
      // Someone open and closed the door, then they probably passed the door;
      in = !in;
      sendCommand();
    }
    prevDoorOpen = doorOpen;
    Serial.println(in);
    Serial.println(thres);
    Serial.println(sensorValue);
    Serial.println();
    delay(1000);
}

