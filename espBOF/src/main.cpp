/*
  Program that is vulnerable to buffer overflow.
  Made by Bangedaon (Arjan Vreugdenhil)
*/
#include <Arduino.h>

int ledBlue = LED_BUILTIN; // gpio16; on mainboard
int ledOther = 2;          // gpio2; on chip

void displayFlag() { Serial.println("Here's the flag: ctf{FuckYeahMCUBOF}"); }

char readChar() {
  char buf[1];
  Serial.readBytes(buf, 1);
  return buf[0];
}

void readUserData() {
  char buf[8];
  int index = 0;
  for (;;) {
    char inputByte = readChar();
    if (inputByte == '%') {
      index = 0;
      continue;
    } else if (inputByte == '#') {
      buf[index] = '\0';
      break;
    }
    buf[index] = inputByte;
    index++;
  }
  String s = String(buf);
  Serial.println(s);
}

void setup() {
  pinMode(ledBlue, OUTPUT);
  pinMode(ledOther, OUTPUT);
  digitalWrite(ledBlue, HIGH);  // Low for on
  digitalWrite(ledOther, HIGH); // High for off
  Serial.begin(9600);
  Serial.println("Starting!");
  Serial.print("Flag Method Address: ");
  Serial.println((unsigned long)&displayFlag, HEX);
}

unsigned int previousMillis;
unsigned int cycletime = 400;
bool state = false;
void loop() {
  unsigned int currentMillis = millis();
  if (currentMillis > previousMillis + cycletime) {
    state = !state;
    digitalWrite(ledBlue, state);
    digitalWrite(ledOther, !state);
    previousMillis = currentMillis;
  }

  if (Serial.available()) {
    Serial.println("breaking to receive user input");
    readUserData();
  }
}