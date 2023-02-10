#include <WiFiEspAT.h>
#include "Z21.hpp"

WiFiUDP Udp;
Z21 z21(Udp);

int incomingByte = 0; // for incoming serial data
byte packetBuffer[15]; //buffer to hold incoming packet


#define AT_BAUD_RATE 115200

void setup() {

  Serial.begin(115200);
  while (!Serial);
  Serial.setTimeout(10);

  Serial.println("Connecting to WiFi");

  Serial3.begin(AT_BAUD_RATE);
  WiFi.init(Serial3);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println();
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  WiFi.begin("Pompos", "Apa44Bal");
  Serial.println("Connected to WiFi");

  Udp.begin(21105);

  Udp.beginPacket(z21ip, z21port);
  //const byte data[] = { 0x07, 0x00, 0x40, 0x00, 0x21, 0x81, 0xa0 };
  const byte data[] = { 0x04, 0x00, 0x10, 0x00};
  Udp.write(data, sizeof(data));
  Udp.endPacket();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  handleSerial();
  handleUDP();
}

void handleSerial() {
  if (Serial.available() > 0) {     //wait for data available
    String teststr = Serial.readString();  //read until timeout
    teststr.trim();                        // remove any \r \n whitespace at the end of the String

    if (teststr == "stop") {
      z21.stop();
    }
    else if (teststr == "on") {
      z21.dccOn();
    } 
    else if (teststr == "turn") {
      while (Serial.available() == 0) {}     //wait for data available
      String teststr = Serial.readString();  //read until timeout
      teststr.trim();                        // remove any \r \n whitespace at the end of the String
      z21.setTurnoutStraight(teststr.toInt());
    } 
    else {
      Serial.println("Something else");
    }
  }
}

void handleUDP() {
// if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 15);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    for (size_t i = 0; i < packetSize; i++)
    {
      Serial.print(packetBuffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    

    // send a reply, to the IP address and port that sent us the packet we received
    /*Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();*/
    digitalWrite(LED_BUILTIN, LOW);
  }
}