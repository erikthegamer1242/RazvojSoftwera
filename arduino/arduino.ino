#include <LoRa.h>
#include "LoRaMesh.h"

unsigned long startT;
unsigned long currT;
unsigned long endT = 5e3;

LoRaMesh loramesh;
void setup() {
    Serial.begin(115200);
    loramesh.loraSetup(2);
}

void loop() {
    currT = millis();
    int packetSize = loramesh.parsePacket();
    if (packetSize)
    {
        Serial.println(packetSize);
        loramesh.meshRead();
    }
    if (currT - startT >= endT)
    {
        startT = millis();
        endT = 5e3; // MAKNUTI
        Serial.println("Timeout!");
        loramesh.helloPacket(loramesh.createRoutingPacket());
    }
    delay(5);
}