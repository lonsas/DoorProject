#include "Arduino.h"
int espEnPin = 3;
int statusPin = 4;
void setup() {
    Serial.begin(9600);
    pinMode(espEnPin, OUTPUT);
    digitalWrite(espEnPin, HIGH);
      pinMode(statusPin, OUTPUT);
  digitalWrite(statusPin, LOW);
}

void loop() {
    Serial.write("AT+CWJAP=\"topshop\",\"#Pelargon9Bk\"\r\n");
    delay(10000);
    digitalWrite(statusPin, HIGH);
    delay(1000);
    digitalWrite(statusPin, LOW);
}
