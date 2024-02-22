#include <SPI.h>
#include <LoRa.h>
#include <LoRaMesh.h>
#include "DebugHelper.h"
#include "Helper.h"


#if defined(CONFIG_IDF_TARGET_ESP32C3)
#pragma message "Using ESP32C3"
#define ssPin 5
#define rst 6
#define dio0 7
#define SCK 4
#define MISO 1
#define MOSI 10
#define C3 1

#elif defined(CONFIG_IDF_TARGET_ESP32)
#pragma message "Using ESP32"
#define ssPin 5
#define rst 27
#define dio0 33
#define C3 0

#else
#error "Wrong board!"
#endif



unsigned long startT;
unsigned long currT;
unsigned long endT = 5e3;

bool debugging = false;

LoRaMesh loramesh;

void readAckCallback(int msgId, int from)
{
  String ackForMsgID = "a:" + String(msgId) + ":" + String(from);
  Serial.println(ackForMsgID);
}

void readMsgCallback(int from, int msgId, String msg)
{
  String message = "r:" + String(from) + ":" + String(msgId) + ":" + msg;
  Serial.println(message);
}

void setup()
{
  Serial.begin(115200);
  String myID = "";
  do
  {
    Serial.println("Please input this devices ID next, or inputted value is not a number.");
    while (Serial.available() == 0) {}
    myID = Serial.readString();
    myID.trim();
  } while (!isNumeric(myID));
  int myID_Int = myID.toInt();
  debugPrintln(myID);
  if (C3) SPI.begin(SCK, MISO, MOSI, SS);
  loramesh.readMsgCallback(readMsgCallback);
  loramesh.readAckCallback(readAckCallback);
  loramesh.debugging = debugging;
  if(!loramesh.loraSetup(myID_Int, ssPin, rst, dio0)) 
  {
    Serial.println("LoRa init failed. Check your connections.");
    while (true) {}
  }
  debugPrintln("LoRa init succeeded.");
}

void loop()
{
  currT = millis();
  int packetSize = loramesh.parsePacket();

  if (packetSize)
  {
    loramesh.meshRead();
  }
  if (currT - startT >= endT)
  {
    startT = millis();
    endT = 5e3; // MAKNUTI
    loramesh.routingPacket();
  }
  delay(5);
  // Command menu
  if (Serial.available() > 0)
  {
    String incoming = Serial.readString();
    incoming.trim();
    menu(incoming);
  }
}
