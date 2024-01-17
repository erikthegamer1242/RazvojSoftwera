#ifndef LORAMESH_H
#define LORAMESH_H

#include <SPI.h>
#include <LoRa.h>
#include <map>
#include <utility>
#include <vector>

#define ssPin 5
#define rst 14
#define dio0 2


class LoRaMesh : public LoRaClass{
private:
  unsigned short int myID;
  unsigned long startT = millis(), currT=startT,endT=10e3;
public:
    byte typeDataMessage = 0, typeRoutingMessage = 1, typeAckMessage = 6;
    struct packet{
        unsigned short int source, destination, nextHop, packetSize, msgId;
        byte type, preamble;
        char payload[244];
    };//sendPacket, receivePacket;

    String receivedMsg;
    char payload[244];
    bool debuging = true;
    std::map<unsigned short int,std::pair<unsigned short int,unsigned short int>>routingTable;
    std::map<unsigned short int,unsigned short int>neighbours;
    std::vector<unsigned short int>knownRoutes;
    void loraSetup(int myID);
    unsigned short int readInt();
    void writeInt(unsigned short int num);
    void debug(packet p);
    void meshRead();
    packet createMessagePacket(String msg, int idToSend, int msgId, int packetType = 0);
    packet createAckPacket(int sizeOfMsgToAck, int idToSend, int msgIdToAck);
    packet createRoutingPacket();
    void meshSend(packet sendPacket);
    int getmyID();
    void helloPacket(packet sendPacket);
    void updateRoutingTable(int id, unsigned short int size);
    void printRoutingTable();
};

#endif
