#ifndef LORAMESH_H
#define LORAMESH_H

#include <SPI.h>
#include <LoRa.h>
#include <map>
#include <utility>
#include <vector>

/**
 * @brief The LoRaMesh class represents a mesh network using LoRa communication.
 * 
 * This class extends the LoRaClass and provides additional functionality for creating and managing a mesh network.
 * It includes methods for setting up the LoRa module, sending and receiving packets, managing routing tables, and debugging.
 * 
 * The LoRaMesh class also defines a packet structure for sending and receiving messages within the mesh network.
 * It includes fields for the source, destination, next hop, packet size, message ID, type, preamble, and payload.
 * 
 * The class also provides callback functions for handling received data messages and acknowledgment messages.
 * 
 * @note This class assumes the use of the LoRa library by Sandeep Mistry (https://github.com/sandeepmistry/arduino-LoRa) 
 *       and requires the LoRa module to be properly configured.
 */
class LoRaMesh : public LoRaClass{
private:
  unsigned short int myID; /**< ID of this node. */
  std::function<void(int, int, String)> callbackDataFunction; /**< Registered user function for handling messages. */
  std::function<void(int, int)> callbackAckFunction; /**< Registered user function for handling acknolegments. */
  bool callbackDataFlag = false; /**< Flag to indicate if a callback function for data messages is registered. */
  bool callbackAckFlag = false; /**< Flag to indicate if a callback function for acknowledgment messages is registered. */
public:
    byte typeDataMessage = 0, /**< Data type packet. */
    typeRoutingMessage = 1, /**< Routing type packet. */
    typeAckMessage = 6; /**< Acknowledgment type packet. */
    /**
     * @brief Structure representing a packet in the LoRaMesh network.
     */
    struct packet{
      unsigned short int source; /**< Sender nodeID. */
      unsigned short int destination; /**< Receiver nodeID. */
      unsigned short int nextHop; /**< Next hop nodeID. */
      unsigned short int packetSize; /**< Size of the packet. */
      unsigned short int msgId = 2; /**< Message ID. */
      byte type; /**< Type of the packet. */
      byte preamble; /**< Preamble. */
      char payload[244]; /**< Packet data. */
    };
    String receivedMsg; /**< Received message. */
    char payload[244]; /**< Packet data. */
    bool debugging = true; /**< Debugging flag. */
    std::map<unsigned short int,std::pair<unsigned short int,unsigned short int>>routingTable; /**< Routing table containing nodeID, nextHop, and hopCount. */
    std::map<unsigned short int,unsigned short int>neighbours; /**< Neighbour table containing nodeID and hopCount. */ 
    std::vector<unsigned short int>knownRoutes; /**< A table containing known routes by saving their nodeID. */
    int loraSetup(int myID, int ssPin, int rst, int dio0);
    unsigned short int readInt();
    void writeInt(unsigned short int num);
    void debug(packet p);
    void meshRead();
    packet createMessagePacket(String msg, int idToSend, int msgId, int packetType = 0);
    packet createAckPacket(int sizeOfMsgToAck, int idToSend, int msgIdToAck);
    void meshSend(packet sendPacket);
    int getmyID();
    void routingPacket();
    void updateRoutingTable(int id, unsigned short int size);
    void printRoutingTable();
    template <typename T> void debugPrint(T input);
    template <typename T> void debugPrintln(T input);
    void debugPrintln();
    void readMsgCallback(std::function<void (int, int, String)> callbackData);
    void readAckCallback(std::function<void (int, int)> callbackAck);
};

#endif
