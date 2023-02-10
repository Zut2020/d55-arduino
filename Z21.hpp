#ifndef Z21_h
#define Z21_h

#include <Arduino.h>
#include <WiFiEspAT.h>

WiFiUdpSender Udp;

const IPAddress z21ip(192, 168, 1, 35);
const int z21port = 21105;

class Z21 {
    private:
        void sendData(const byte data[], int size) {
            Udp.beginPacket(z21ip, z21port);
            Udp.write(data, size);
            Udp.endPacket();
        }

        void addXOR(byte data[], int size) {
            byte xorb = 0;
            for (size_t i = 0; i < size - 1; i++)
            {
                xorb = xorb ^ data[i];
            }
            
            data[size - 1] = xorb;
        }
    public:
        Z21() {
            Udp.begin(z21port);
        }

        void xorTest() {
            byte data[] = { 0x09, 0x00, 0x40, 0x00, 0x53, 0x00, (byte)(2), 0b10001001, 0x00, 0x00 };
            addXOR(data, sizeof(data));
            Serial.println(data[sizeof(data)-1]);
        }

        void setTurnoutStraight(WiFiUdpSender Udp, byte address) {
            byte data[] = { 0x09, 0x00, 0x40, 0x00, 0x53, 0x00, (byte)(address), 0b10001001, 0x00, 0x00 };
            addXOR(data, sizeof(data));
            sendData(data, sizeof(data));
            Serial.println("Turnout set");
        }

        void dccOn(WiFiUdpSender Udp) {
            const byte data[] = { 0x07, 0x00, 0x40, 0x00, 0x21, 0x81, 0xa0 };
            sendData(data, sizeof(data));
            Serial.println("DCC on");
        }

        void stop(WiFiUdpSender Udp) {
            const byte data[] = { 0x06, 0x00, 0x40, 0x00, 0x80, 0x80 };
            sendData(data, sizeof(data));
            Serial.println("Stopped");
        }
};

#endif