#include "LoRaMesh.h"
#include <SPI.h>
#include <LoRa.h>
#include <iterator>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <stdio.h>


char message[244] = "Hello World";

void LoRaMesh::loraSetup(int myID) {
    Serial.print("loraSetup(");
    Serial.print(myID);
    Serial.println(")");
    
    this->myID = myID;
    setPins(ssPin, rst, dio0);
    if (!begin(868E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    Serial.println("Starting LoRa success!");
    routingTable[this->myID] = {this->myID, 0};
    knownRoutes.push_back(this->myID);
}

int LoRaMesh::getmyID() {
//    Serial.print("getmyID(");
//    Serial.print("");
//    Serial.println(")");
    
    return myID;
}

unsigned short int LoRaMesh::readInt() {
    Serial.print("readInt(");
    Serial.print("");
    Serial.println(")");
    
    byte low = (byte) read();
    byte high = (byte) read();
    delay(10);
    return (high << 8 | low);
}

void LoRaMesh::writeInt(unsigned short int num) {
    Serial.print("writeInt(");
    Serial.print(num);
    Serial.println(")");
    
    byte high = highByte(num);
    byte low = lowByte(num);
    write(low);
    write(high);
}

void LoRaMesh::debug(packet p) {
    Serial.print("packet(");
    Serial.print("p");
    Serial.print(") ");
    Serial.print("debuging = ");
    Serial.println(debuging);
    
    if (!debuging) {
        return;
    }
    Serial.print("Preamble: ");
    Serial.println(p.preamble);
    Serial.print("Destination: ");
    Serial.println(p.destination);
    Serial.print("Source: ");
    Serial.println(p.source);
    Serial.print("Next Hop: ");
    Serial.println(p.nextHop);
    Serial.print("Type: ");
    Serial.println(p.type);
    Serial.print("Packet Size: ");
    Serial.println(p.packetSize);
    Serial.print("Payload: ");
    for (int i = 0; i < p.packetSize; ++i) {
        Serial.print(p.payload[i]);
    }
    Serial.print("\nMsgId: ");
    Serial.println(p.msgId);
    Serial.println();
}

void LoRaMesh::printRoutingTable() {
    Serial.print("printRoutingTable(");
    Serial.print("");
    Serial.println(")");
    
    for (const auto &x : routingTable) {
        Serial.print(x.first);
        Serial.print(" ");
        Serial.print(x.second.first);
        Serial.print(" ");
        Serial.println(x.second.second);
    }
}

void LoRaMesh::updateRoutingTable(int id, unsigned short int size) {
    Serial.print("updateRoutingTable");
    Serial.print(id);
    Serial.print(", ");
    Serial.print(size);
    Serial.println(")");
    
    std::vector<unsigned short int> vNew;
    for (int i = 0; i < size / 3; ++i) {
        int nextHop = readInt();
        int nodeID = readInt();
        int newRssi = readInt();
        vNew.push_back(nodeID);
        Serial.print(nextHop);
        Serial.print(" ");
        Serial.print(nodeID);
        Serial.print(" ");
        Serial.print(newRssi);
        Serial.println();

        if (routingTable.find(id) == routingTable.end())
            routingTable[nodeID] = {id, newRssi};
        else {
            if (routingTable[nodeID].second > newRssi)
                routingTable[nodeID] = {id, newRssi};
        }

    }
    
    for (const auto &x : routingTable){
        if (x.second.first == id){
            if (std::find(vNew.begin(),vNew.end(),x.first) == vNew.end())
                routingTable.erase(x.first);
        }
    }

    if(neighbours.find(id) == neighbours.end())
        neighbours.insert({id, 0});
    else
        neighbours[id] = 0;

    printRoutingTable();
}

void LoRaMesh::meshRead() {
    Serial.print("meshRead(");
    Serial.print("");
    Serial.println(")");
    
    packet receivePacket;
    receivePacket.preamble = (byte) read();
    if (receivePacket.preamble != 170)
        return;

    receivePacket.destination = readInt();
    receivePacket.source = readInt();
    receivePacket.nextHop = readInt();
    receivePacket.type = (byte) read();
    receivePacket.packetSize = readInt();

    if (receivePacket.type == typeRoutingMessage) {
        Serial.println("typeRoutingMessage Receved!");
        updateRoutingTable(receivePacket.source, receivePacket.packetSize);
        return;
    }

    for (int i = 0; i < (int) receivePacket.packetSize; ++i) {
        receivePacket.payload[i] = (char) (((byte) read()) - 1);
        delay(10);
    }
    receivePacket.msgId = readInt();
    receivePacket.payload[receivePacket.packetSize] = '\0';
    receivedMsg = String(receivePacket.payload);
    debug(receivePacket);

    // if (receivePacket.destination == myID) {
        // if (receivePacket.type == typeAckMessage) {
        //     Serial.println("typeAckMessage Receved!");
        //     appendFile3(SPIFFS, ("/" + String(receivePacket.source) + ".ack").c_str(), (String(receivePacket.msgId) + '\n').c_str());
        //     if (receivePacket.source == idToSend && receivePacket.msgId < msgElements.size())
        //     {
        //         lv_obj_set_style_bg_color(msgElements[receivePacket.msgId], lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
        //         lv_obj_set_style_border_color(msgElements[receivePacket.msgId], lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
        //     }
        //     return;
        // }
        // Serial.println("typeDataMessage Receved!");
        // if(idToSend != 0){
        //     msgElements.push_back(ui_IncomingMessage_create(ui_Messages));
        //     lv_textarea_set_text(msgElements[msgElements.size() - 1], loramesh.receivedMsg.c_str());
        //     lv_obj_scroll_to_y(ui_Messages, -100000000, LV_ANIM_OFF);
        // }
        // String pathMsg;
        // pathMsg ="/" + (String(receivePacket.source) + ".txt");
        // String msg = ">" + String(receivePacket.payload) + "\t";

        // Serial.printf("pathMsg = %s\n", pathMsg);
        // Serial.printf("String(receivePacket.payload).c_str() = %s\n",  String(receivePacket.payload).c_str());

        // appendFile3(SPIFFS, pathMsg.c_str(), msg.c_str());
        // int msgIdToSend = (readFile3(SPIFFS, ("/" + String(receivePacket.source) + ".lid").c_str())).toInt();
        // writeFile3(SPIFFS, ("/" + String(receivePacket.source) + ".lid").c_str(), String(msgIdToSend + 1).c_str());

        // pinMode(2, OUTPUT);
	    // digitalWrite(2, HIGH);
	    // delay(150);
	    // digitalWrite(2, LOW);

        // delay(500);
        // meshSend(createAckPacket(receivePacket.packetSize, receivePacket.source, receivePacket.msgId));
        
        // return;
    // }
    if (receivePacket.nextHop != myID)
        return;
    if (receivePacket.type == typeDataMessage) {
        receivePacket.nextHop = routingTable[receivePacket.destination].first;
        delay(2500);
        meshSend(receivePacket);
    }
    return;
}

LoRaMesh::packet LoRaMesh::createMessagePacket(String msg, int idToSend, int msgId, int packetType)
{
    Serial.print("createMessagePaclet(");
    Serial.print(msg);
    Serial.print(", ");
    Serial.print(idToSend);
    Serial.print(", ");
    Serial.print(msgId);
    Serial.println(")");

    packet Packet;
    Packet.preamble = (byte) 170;
    Packet.destination = idToSend;
    Packet.source = myID;
    Packet.nextHop = routingTable[Packet.destination].first;
    Packet.type = (byte) packetType;
    Packet.packetSize = msg.length();
    msg.toCharArray(Packet.payload, 244);
    Serial.print("payload = ");
    for (int i = 0; i < Packet.packetSize; ++i)
        Serial.print(Packet.payload[i]);
    Packet.msgId = msgId;
    Serial.printf("Packet.msgId = %d\n", Packet.msgId);
    return Packet;
}

void LoRaMesh::meshSend(packet sendPacket) {
    Serial.print("meshSend(");
    debug(sendPacket);
    Serial.println(")");
    
    beginPacket();
    write(sendPacket.preamble);
    writeInt(sendPacket.destination);
    writeInt(sendPacket.source);
    writeInt(sendPacket.nextHop);
    write(sendPacket.type);
    writeInt(sendPacket.packetSize);
    for (int i = 0; i < sendPacket.packetSize; ++i) {
        write((byte) ((int)sendPacket.payload[i] + 1));
    }
    writeInt(sendPacket.msgId);
    Serial.print("endPacket() = ");
    Serial.println(endPacket(1));
    return;
}

LoRaMesh::packet LoRaMesh::createRoutingPacket() {
    Serial.print("createRoutingPacket(");
    Serial.print("");
    Serial.println(")");
    
    packet Packet;
    Packet.preamble = (byte) 170;
    Packet.destination = 0;
    Packet.source = myID;
    Packet.nextHop = 0; // 0 je broadcast
    Packet.type = (byte) typeRoutingMessage;
    Packet.packetSize = knownRoutes.size() * 3;
    Packet.payload[0] = '\0';
    return Packet;
}


void LoRaMesh::helloPacket(LoRaMesh::packet sendPacket) {
    Serial.print("helloPacket(");
    debug(sendPacket);
    Serial.println(")");
    
    for (const auto &x : neighbours) {
        neighbours[x.first]++;
    }
    
    Serial.print("neighbours = ");
    for (const auto &x : neighbours) {
        Serial.print("(");
        Serial.print(x.first);
        Serial.print(", ");
        Serial.print(x.second);
        Serial.print("), ");
        if (x.second >= 40000) {// VRATITI NA 4
            for(const auto &y : routingTable)
                if(y.second.first == x.first)
                    routingTable.erase(y.first);
            neighbours.erase(x.first);
            printRoutingTable();
        }
    }
    Serial.println();
    
    beginPacket();
    write(sendPacket.preamble);
    writeInt(sendPacket.destination);
    writeInt(sendPacket.source);
    writeInt(sendPacket.nextHop);
    write(sendPacket.type);
    writeInt(sendPacket.packetSize);
    Serial.println("knownRoutes = (");

    for (const auto &x : routingTable){
        Serial.print(x.first);
        Serial.print(", ");
        writeInt(x.first);
        writeInt(x.second.first);
        writeInt(x.second.second);
    }
    Serial.println(")");
    Serial.print("endPacket() = ");
    Serial.println(endPacket(1));
    return;
}

LoRaMesh::packet LoRaMesh::createAckPacket(int sizeOfMsgToAck, int idToSend, int msgIdToAck)
{
    Serial.print("createAckPacket(");
    Serial.print(sizeOfMsgToAck);
    Serial.print(", ");
    Serial.print(idToSend);
    Serial.print(", ");
    Serial.print(msgIdToAck);
    Serial.println(")");

    return createMessagePacket(String(sizeOfMsgToAck), idToSend, msgIdToAck, typeAckMessage);
}
