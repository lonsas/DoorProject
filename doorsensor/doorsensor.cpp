#include "Arduino.h"
#include "LowPower.h"
//#define SERIALDEBUG
//#define LEDDEBUG
#define MAX_CONNECT_TRIES 10

int sensorPin = A0;
int sensorEnPin = 5;
int espEnPin = 3;
int ledPin = 2;
int statusPin = 4;
int sensorValue = 0;
int thres = 162;
bool doorOpen = false;
bool prevDoorOpen = false;
bool in = true;
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(sensorEnPin, OUTPUT);
  digitalWrite(sensorEnPin, LOW);
  pinMode(espEnPin, OUTPUT);
  digitalWrite(espEnPin, LOW);
  pinMode(statusPin, OUTPUT);
  digitalWrite(statusPin, LOW);
}

void sendCommand() {

    digitalWrite(espEnPin, HIGH);
    int tries = 0;
    do {
        Serial.write("AT+CIPSTART=\"TCP\",\"192.168.1.2\",8000\r\n");
        delay(50);
        tries++;
    } while(!Serial.find("CONNECT") && tries < MAX_CONNECT_TRIES);
    Serial.write("AT+CIPSEND=8\r\n");
    delay(50);
    Serial.write("DOOROPEN");
    Serial.write("AT+CIPCLOSE\r\n");
    delay(200);
   
    digitalWrite(espEnPin, LOW);
}

void loop() {
    digitalWrite(sensorEnPin, HIGH);
    digitalWrite(ledPin, HIGH);
    // Give the led time to shine, doesn't work otherwise
    delay(40);
    sensorValue = analogRead(sensorPin);
    digitalWrite(ledPin, LOW);
    digitalWrite(sensorEnPin, LOW);
    doorOpen = sensorValue < thres;
#ifdef LEDDEBUG
    if(!doorOpen) {
      digitalWrite(statusPin, HIGH);
      delay(100);
      digitalWrite(statusPin, LOW);
    } else {
      digitalWrite(statusPin, HIGH);
      delay(1000);
      digitalWrite(statusPin, LOW);
    }
#endif
    if(!prevDoorOpen && doorOpen) {
      sendCommand();
    }
    prevDoorOpen = doorOpen;
#ifdef SERIALDEBUG
    while(Serial1.available()) {
        Serial.write(Serial1.read());
    }
    Serial.println(thres);
    Serial.println(sensorValue);
    Serial.println();
 #endif
    LowPower.idle(SLEEP_2S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
}

