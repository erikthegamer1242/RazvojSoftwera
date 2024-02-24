#include <SPI.h>
#include <LoRa.h>
#include <LoRaMesh.h>
#include "DebugHelper.h"
#include "Helper.h"

/** Pin mapping of the LoRa module based on the board used
 * for compiling the code.
 */
#if defined(CONFIG_IDF_TARGET_ESP32C3)
#pragma message "Using ESP32C3"
#define ssPin 5
#define rst 6
#define dio0 7
#define SCK 4
#define MISO 1
#define MOSI 10
/** Considering we are using an ESP WROVER E module and the C3 
 * mini module, both get detected as the ESP32 so we check for 
 * CONFIG_IDF_TARGET_ESP32C3 before and set this flag if its 
 * the C3 mini used later in the code
 * */
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

unsigned long startT; /**< Variable to store the start time of the loop */
unsigned long currT; /**< Variable to store the current time of the loop */
unsigned long endT = 5e3; /**< The time between sending each routing packet */

bool debugging = false;

LoRaMesh loramesh;

/**
 * Callback function that handles the received acknowledgment packet.
 * 
 * @param [in] msgId The ID of the message.
 * @param [in] from The sender of the acknowledgment.
 */
void readAckCallback(int msgId, int from)
{
  String ackForMsgID = "a:" + String(msgId) + ":" + String(from);
  Serial.println(ackForMsgID);
}

/**
 * Callback function that handles the received message packet.
 * 
 * @param from The sender of the message.
 * @param msgId The ID of the message.
 * @param msg The content of the message.
 */
void readMsgCallback(int from, int msgId, String msg)
{
  String message = "r:" + String(from) + ":" + String(msgId) + ":" + msg;
  Serial.println(message);
}

/**
 * Function to setup the LoRa module and the serial monitor.
 * The function will prompt the user to input the ID of the device.
 * and then initialize the LoRa module with the given ID.
 * 
 */
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

/**
 * Function that is called in a loop to handle the routing of the packets
 * and the user input from the serial monitor. Also, it checks for incoming
 * packets and reads them.
 * 
 */
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
