#include "LoRaMesh.h"
#include <SPI.h>
#include <LoRa.h>
#include <iterator>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <stdio.h>

/**
 * A function to print debug messages to the serial monitor
 * based on a global debugging flag
 * 
 * @param [in] print The string to be printed
 * 
 */
template <typename T>
void LoRaMesh::debugPrint(T print)
{
    if (debugging)
        Serial.print(print);
}

/**
 * A function to print debug messages to the serial monitor with new line
 * based on a global debugging flag
 *
 * @param [in] print The string to be printed
 * 
 */
template <typename T>
void LoRaMesh::debugPrintln(T print)
{
    if (debugging)
        Serial.println(print);
}

/**
 * A function to print debug messages to the serial monitor with new line
 * and no input, aka. empty line based on a global debugging flag
 * 
 */
void LoRaMesh::debugPrintln()
{
    if (debugging)
        Serial.println();
}

/**
 * This function extends the default one from the LoRa library
 * adding all of the necessary parameters for the LoRaMesh class
 * and setting the necessary pins for the LoRa module
 * to function
 *
 * @param[in] myID ID to be used by this node
 * @param[in] ssPin The slave select pin for the LoRa module
 * @param[in] rst The reset pin for the LoRa module
 * @param[in] dio0 The interrupt pin for the LoRa module
 * @returns 1 if the LoRa module is successfully started, 0 otherwise
 */
int LoRaMesh::loraSetup(int myID, int ssPin, int rst, int dio0)
{
    debugPrint("loraSetup(");
    debugPrint(myID);
    debugPrintln(")");

    this->myID = myID;
    setPins(ssPin, rst, dio0);
    if (!begin(868E6)) return 0;
    routingTable[this->myID] = {this->myID, 0};
    knownRoutes.push_back(this->myID);
    return 1;
}

/**
 * Register a callback function to be called when a 
 * message is received so that any user can interpret the message as they want
 *
 * @param[in] callbackData The callback function to register.
 * 
 */
void LoRaMesh::readMsgCallback(std::function<void(int, int, String)> callbackData) //
{
    callbackDataFunction = callbackData;
    debugPrintln("readMsgCallback");
}

/**
 * Register a callback function to be called when a acknowlegment is 
 * received so that any user can interpret the acknowlegment as they want
 *
 * @param callbackAck The callback function to register.
 */
void LoRaMesh::readAckCallback(std::function<void(int, int)> callbackAck)
{
    callbackAckFunction = callbackAck;
    debugPrintln("readAckCallback");
}

/**
 * Getter for the ID of this node
 *
 * @returns The ID of this node
 */
int LoRaMesh::getmyID()
{
    return myID;
}

/**
 * Used for reading an integer received from the LoRa module
 *
 * @returns The received integer
 */
unsigned short int LoRaMesh::readInt()
{
    debugPrint("readInt(");
    debugPrint("");
    debugPrintln(")");

    byte low = (byte)read();
    byte high = (byte)read();
    delay(10);
    return (high << 8 | low);
}

/**
 * Used for writing an integer to the LoRa module
 *
 * @param[in] num The integer to be sent
 * 
 */
void LoRaMesh::writeInt(unsigned short int num)
{
    debugPrint("writeInt(");
    debugPrint(num);
    debugPrintln(")");

    byte high = highByte(num);
    byte low = lowByte(num);
    write(low);
    write(high);
}

/**
 * Used for debugging the packet received from the LoRa module
 * and printing it to the serial monitor
 *
 * @param[in] p The packet to be debugged
 * 
 */
void LoRaMesh::debug(packet p)
{
    debugPrint("packet(");
    debugPrint("p");
    debugPrint(") ");
    debugPrint("debugging = ");
    debugPrintln(debugging);

    if (!debugging)
    {
        return;
    }
    debugPrint("Preamble: ");
    debugPrintln(p.preamble);
    debugPrint("Destination: ");
    debugPrintln(p.destination);
    debugPrint("Source: ");
    debugPrintln(p.source);
    debugPrint("Next Hop: ");
    debugPrintln(p.nextHop);
    debugPrint("Type: ");
    debugPrintln(p.type);
    debugPrint("Packet Size: ");
    debugPrintln(p.packetSize);
    debugPrint("Payload: ");
    for (int i = 0; i < p.packetSize; ++i)
    {
        debugPrint(p.payload[i]);
    }
    debugPrint("\nMsgId: ");
    debugPrintln(p.msgId);
    debugPrintln();
}

/**
 * Used for printing the routing table to the serial monitor
 *
 */
void LoRaMesh::printRoutingTable()
{
    debugPrint("printRoutingTable(");
    debugPrint("");
    debugPrintln(")");

    for (const auto &x : routingTable)
    {
        Serial.print(x.first);
        Serial.print(" ");
        Serial.print(x.second.first);
        Serial.print(" ");
        Serial.println(x.second.second);
    }
}

/**
 * A function to update the routing table with a new node
 * 
 * @param[in] id The ID of the node to be added to the routing table
 * @param[in] size The size of the packet received
 * 
 */
void LoRaMesh::updateRoutingTable(int id, unsigned short int size)
{
    debugPrint("updateRoutingTable");
    debugPrint(id);
    debugPrint(", ");
    debugPrint(size);
    debugPrintln(")");

    std::vector<unsigned short int> vNew;
    for (int i = 0; i < size / 3; ++i)
    {
        int nextHop = readInt();
        int nodeID = readInt();
        int newRssi = readInt();
        vNew.push_back(nodeID);
        debugPrint(nextHop);
        debugPrint(" ");
        debugPrint(nodeID);
        debugPrint(" ");
        debugPrint(newRssi);
        debugPrintln();

        if (routingTable.find(nodeID) == routingTable.end())
            routingTable[nodeID] = {id, newRssi};
        else
        {
            if (routingTable[nodeID].second > newRssi)
                routingTable[nodeID] = {id, newRssi};
        }
    }

    for (const auto &x : routingTable)
    {
        if (x.second.first == id)
        {
            if (std::find(vNew.begin(), vNew.end(), x.first) == vNew.end())
            {
            }
            // routingTable.erase(x.first);
        }
    }

    if (neighbours.find(id) == neighbours.end())
        neighbours.insert({id, 0});
    else
        neighbours[id] = 0;

    if (debugging) printRoutingTable();
}

/**
 * A function to read and interpret the packet received from the LoRa module,
 * this is where we use the callback functions to interpret the message
 * 
 * 
 */
void LoRaMesh::meshRead()
{
    debugPrint("meshRead(");
    debugPrint("");
    debugPrintln(")");

    packet receivePacket;
    receivePacket.preamble = (byte)read();
    if (receivePacket.preamble != 170)
        return;

    receivePacket.destination = readInt();
    receivePacket.source = readInt();
    receivePacket.nextHop = readInt();
    receivePacket.type = (byte)read();
    receivePacket.packetSize = readInt();

    if (receivePacket.type == typeRoutingMessage)
    {
        debugPrintln("typeRoutingMessage Receved!");
        updateRoutingTable(receivePacket.source, receivePacket.packetSize);
        return;
    }

    for (int i = 0; i < (int)receivePacket.packetSize; ++i)
    {
        receivePacket.payload[i] = (char)(((byte)read()) - 1);
        delay(10);
    }
    receivePacket.msgId = readInt();
    receivePacket.payload[receivePacket.packetSize] = '\0';
    receivedMsg = String(receivePacket.payload);
    debug(receivePacket);

    if (receivePacket.destination == myID)
    {
        if (receivePacket.type == typeAckMessage)
        {
            debugPrintln("typeAckMessage Receved!");
            callbackAckFunction(receivePacket.msgId, receivePacket.source);
            return;
        }
        if (receivePacket.type == typeDataMessage)
        {
            debugPrintln("typeDataMessage Receved!");
            callbackDataFunction(receivePacket.source, receivePacket.msgId, receivedMsg);
            delay(500);
            meshSend(createAckPacket(receivePacket.packetSize, receivePacket.source, receivePacket.msgId));
        }
    }
    if (receivePacket.nextHop != myID)
        return;
    if (receivePacket.type == typeDataMessage)
    {
        receivePacket.nextHop = routingTable[receivePacket.destination].first;
        delay(2500);
        meshSend(receivePacket);
    }
    return;
}

/**
 * A function create a new packet
 * 
 * @param[in] msg A string containing the message 
 * @param[in] idToSend The recipient ID
 * @param[in] msgId The message ID
 * @param[in] packetType The type of the packet being sent
 * @returns A packet
 */
LoRaMesh::packet LoRaMesh::createMessagePacket(String msg, int idToSend, int msgId, int packetType)
{
    debugPrint("createMessagePacket(");
    debugPrint(msg);
    debugPrint(", ");
    debugPrint(idToSend);
    debugPrint(", ");
    debugPrint(msgId);
    debugPrintln(")");

    packet Packet;
    Packet.preamble = (byte)170;
    Packet.destination = idToSend;
    Packet.source = myID;
    Packet.nextHop = routingTable[Packet.destination].first;
    Packet.type = (byte)packetType;
    Packet.packetSize = msg.length();
    msg.toCharArray(Packet.payload, 244);
    debugPrint("payload = ");
    for (int i = 0; i < Packet.packetSize; ++i)
        debugPrint(Packet.payload[i]);
    Packet.msgId = msgId;
    if (debugging)
        Serial.printf("Packet.msgId = %d\n", Packet.msgId);
    return Packet;
}

/**
 * Function to send a packet to the LoRa module
 * 
 * @param[in] sendPacket The packet to be sent
 * 
 */
void LoRaMesh::meshSend(packet sendPacket)
{
    debugPrint("meshSend(");
    debug(sendPacket);
    debugPrintln(")");

    beginPacket();
    write(sendPacket.preamble);
    writeInt(sendPacket.destination);
    writeInt(sendPacket.source);
    writeInt(sendPacket.nextHop);
    write(sendPacket.type);
    writeInt(sendPacket.packetSize);
    for (int i = 0; i < sendPacket.packetSize; ++i)
    {
        write((byte)((int)sendPacket.payload[i] + 1));
    }
    writeInt(sendPacket.msgId);
    debugPrint("endPacket() = ");
    debugPrintln(endPacket(1));
    return;
}


/**
 * Sends a routing packet to the LoRa module.
 * This function constructs a packet with routing information and sends it to the LoRa module.
 * It updates the neighbor count and checks for any neighbors with a count above a threshold.
 * If a neighbor's count exceeds the threshold, it removes the neighbor from the routing table and neighbor list.
 * Finally, it sends the packet to the network.
 * 
 * 
 */
void LoRaMesh::routingPacket() //Bivski helloPacket
{
    packet sendPacket;
    sendPacket.preamble = (byte)170;
    sendPacket.destination = 0;
    sendPacket.source = myID;
    sendPacket.nextHop = 0; // 0 je broadcast
    sendPacket.type = (byte)typeRoutingMessage;
    sendPacket.packetSize = knownRoutes.size() * 3;
    sendPacket.payload[0] = '\0';
    debugPrint("helloPacket(");
    debug(sendPacket);
    debugPrintln(")");

    for (const auto &x : neighbours)
    {
        neighbours[x.first]++;
    }

    debugPrint("neighbours = ");
    for (const auto &x : neighbours)
    {
        debugPrint("(");
        debugPrint(x.first);
        debugPrint(", ");
        debugPrint(x.second);
        debugPrint("), ");
        if (x.second >= 4)
        {
            for (const auto &y : routingTable)
                if (y.second.first == x.first)
                    routingTable.erase(y.first);
            neighbours.erase(x.first);
            if (debugging) printRoutingTable();
        }
    }
    debugPrintln();

    beginPacket();
    write(sendPacket.preamble);
    writeInt(sendPacket.destination);
    writeInt(sendPacket.source);
    writeInt(sendPacket.nextHop);
    write(sendPacket.type);
    writeInt(sendPacket.packetSize);
    debugPrintln("knownRoutes = (");

    for (const auto &x : routingTable)
    {
        debugPrint(x.first);
        debugPrint(", ");
        writeInt(x.first);
        writeInt(x.second.first);
        writeInt(x.second.second);
    }
    debugPrintln(")");
    debugPrint("endPacket() = ");
    debugPrintln(endPacket(1));
    return;
}

/**
 * A function to create an acknowledgment packet based on the 
 * message packet
 * 
 * @param[in] sizeOfMsgToAck The size of the message to be acknowledged
 * @param[in] idToSend The recipient ID
 * @param[in] msgIdToAck The message ID to be acknowledged
 * @returns A packet
 * 
 */
LoRaMesh::packet LoRaMesh::createAckPacket(int sizeOfMsgToAck, int idToSend, int msgIdToAck)
{
    debugPrint("createAckPacket(");
    debugPrint(sizeOfMsgToAck);
    debugPrint(", ");
    debugPrint(idToSend);
    debugPrint(", ");
    debugPrint(msgIdToAck);
    debugPrintln(")");

    return createMessagePacket(String(sizeOfMsgToAck), idToSend, msgIdToAck, typeAckMessage);
}
