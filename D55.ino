#include <WiFiEspAT.h>
#include "Z21.hpp"

Z21 z21;

int incomingByte = 0; // for incoming serial data


#define AT_BAUD_RATE 115200

void setup() {

  Serial.begin(115200);
  while (!Serial);
  Serial.setTimeout(10);

  Serial3.begin(AT_BAUD_RATE);
  WiFi.init(Serial3);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println();
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.println("Connecting to WiFi");

  WiFi.begin("Pompos", "Apa44Bal");
  Serial.println("Connected to WiFi");

  pinMode(8, INPUT_PULLUP);
}

void loop() {
  Serial.print(">");
  while (Serial.available() == 0) {}     //wait for data available
  String teststr = Serial.readString();  //read until timeout
  teststr.trim();                        // remove any \r \n whitespace at the end of the String
  if (teststr == "stop") {
    z21.stop(Udp);
  }
  else if (teststr == "on") {
    z21.dccOn(Udp);
  } 
  else if (teststr == "turn") {
    z21.setTurnoutStraight(Udp, 1);
  } 
  else {
    Serial.println("Something else");
  }
}