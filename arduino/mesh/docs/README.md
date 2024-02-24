# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`LoRaMesh`](#classLoRaMesh) | The [LoRaMesh](#classLoRaMesh) class represents a mesh network using LoRa communication.
`struct `[`LoRaMesh::packet`](#structLoRaMesh_1_1packet) | Structure representing a packet in the [LoRaMesh](#classLoRaMesh) network.

# class `LoRaMesh` 

```
class LoRaMesh
  : public LoRaClass
```  

The [LoRaMesh](#classLoRaMesh) class represents a mesh network using LoRa communication.

This class extends the LoRaClass and provides additional functionality for creating and managing a mesh network. It includes methods for setting up the LoRa module, sending and receiving packets, managing routing tables, and debugging.

The [LoRaMesh](#classLoRaMesh) class also defines a packet structure for sending and receiving messages within the mesh network. It includes fields for the source, destination, next hop, packet size, message ID, type, preamble, and payload.

The class also provides callback functions for handling received data messages and acknowledgment messages.

This class assumes the use of the LoRa library by Sandeep Mistry ([https://github.com/sandeepmistry/arduino-LoRa](https://github.com/sandeepmistry/arduino-LoRa)) and requires the LoRa module to be properly configured.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public byte `[`typeDataMessage`](#classLoRaMesh_1a15194ef51873cd5949fde69ede08afeb) | Data type packet.
`public byte `[`typeRoutingMessage`](#classLoRaMesh_1a9d553d8dabc60f6934fd229a40e8844b) | Routing type packet.
`public byte `[`typeAckMessage`](#classLoRaMesh_1adc956ece115b35617c4973f0cfb7b313) | Acknowledgment type packet.
`public String `[`receivedMsg`](#classLoRaMesh_1a285b56d8facaa17d35218bc282c0b851) | Received message.
`public char `[`payload`](#classLoRaMesh_1a5a3a219bdc83318caf199968448428f6) | Packet data.
`public bool `[`debugging`](#classLoRaMesh_1ae48bd2876558f5dd89779239de094573) | Debugging flag.
`public std::map< unsigned short int, std::pair< unsigned short int, unsigned short int > > `[`routingTable`](#classLoRaMesh_1ab0f072af1ee6dc1e8d6680e39a35487e) | Routing table containing nodeID, nextHop, and hopCount.
`public std::map< unsigned short int, unsigned short int > `[`neighbours`](#classLoRaMesh_1a6f6b22519bebac7c727140d8e43879aa) | Neighbour table containing nodeID and hopCount.
`public std::vector< unsigned short int > `[`knownRoutes`](#classLoRaMesh_1a394cbd4e66044c05e06f6557fea78fd0) | A table containing known routes by saving their nodeID.
`public int `[`loraSetup`](#classLoRaMesh_1a10e07d12832de958d1ec023ef01d2320)`(int myID,int ssPin,int rst,int dio0)` | This function extends the default one from the LoRa library adding all of the necessary parameters for the [LoRaMesh](#classLoRaMesh) class and setting the necessary pins for the LoRa module to function
`public unsigned short int `[`readInt`](#classLoRaMesh_1a61e14b5fea402bb78d3865b9973a75c9)`()` | Used for reading an integer received from the LoRa module
`public void `[`writeInt`](#classLoRaMesh_1ab7b9e056b5901c67e8bfbdbfaf35946f)`(unsigned short int num)` | Used for writing an integer to the LoRa module
`public void `[`debug`](#classLoRaMesh_1a0add8dc42f41009fe123c0b8a209f2df)`(`[`packet`](#structLoRaMesh_1_1packet)` p)` | Used for debugging the packet received from the LoRa module and printing it to the serial monitor
`public void `[`meshRead`](#classLoRaMesh_1a18fa94c2dd8c689ee3c53b66bc185d9b)`()` | A function to read and interpret the packet received from the LoRa module, this is where we use the callback functions to interpret the message
`public `[`packet`](#structLoRaMesh_1_1packet)` `[`createMessagePacket`](#classLoRaMesh_1a875f63643e6c5140e655d105ffe32f10)`(String msg,int idToSend,int msgId,int packetType)` | A function create a new packet
`public `[`packet`](#structLoRaMesh_1_1packet)` `[`createAckPacket`](#classLoRaMesh_1a6e4f26a6f526ea6df6f3047221ee3cd9)`(int sizeOfMsgToAck,int idToSend,int msgIdToAck)` | A function to create an acknowledgment packet based on the message packet
`public void `[`meshSend`](#classLoRaMesh_1af7ce1e2757faefd4a44895f157372946)`(`[`packet`](#structLoRaMesh_1_1packet)` sendPacket)` | Function to send a packet to the LoRa module
`public int `[`getmyID`](#classLoRaMesh_1ad34b1123675afe7f74b96ee454d79a59)`()` | Getter for the ID of this node
`public void `[`routingPacket`](#classLoRaMesh_1a3218bc895a74559a4b4f84263451f8f1)`()` | Sends a routing packet to the LoRa module. This function constructs a packet with routing information and sends it to the LoRa module. It updates the neighbor count and checks for any neighbors with a count above a threshold. If a neighbor's count exceeds the threshold, it removes the neighbor from the routing table and neighbor list. Finally, it sends the packet to the network.
`public void `[`updateRoutingTable`](#classLoRaMesh_1a1b22411d4b3b0e2c6f650ba11f7c30ec)`(int id,unsigned short int size)` | A function to update the routing table with a new node
`public void `[`printRoutingTable`](#classLoRaMesh_1a935c7fae1f05090ac9034dd83e98ae02)`()` | Used for printing the routing table to the serial monitor
`public template<>`  <br/>`void `[`debugPrint`](#classLoRaMesh_1a0db56465199a2b8e1aee7bdb20674977)`(T input)` | A function to print debug messages to the serial monitor based on a global debugging flag
`public template<>`  <br/>`void `[`debugPrintln`](#classLoRaMesh_1afad7a1a01d44d7e79c0db6d99bc4b224)`(T input)` | A function to print debug messages to the serial monitor with new line based on a global debugging flag
`public void `[`debugPrintln`](#classLoRaMesh_1a5ac4fe1479114fbccbf68ba1c3ad5ff2)`()` | A function to print debug messages to the serial monitor with new line and no input, aka. empty line based on a global debugging flag
`public void `[`readMsgCallback`](#classLoRaMesh_1a72625ac94eecb0ff02cde7328882412d)`(std::function< void(int, int, String)> callbackData)` | Register a callback function to be called when a message is received so that any user can interpret the message as they want
`public void `[`readAckCallback`](#classLoRaMesh_1a1c3bc1adb648a21d65ba6316f31b1f3a)`(std::function< void(int, int)> callbackAck)` | Register a callback function to be called when a acknowlegment is received so that any user can interpret the acknowlegment as they want

## Members

#### `public byte `[`typeDataMessage`](#classLoRaMesh_1a15194ef51873cd5949fde69ede08afeb) 

Data type packet.

#### `public byte `[`typeRoutingMessage`](#classLoRaMesh_1a9d553d8dabc60f6934fd229a40e8844b) 

Routing type packet.

#### `public byte `[`typeAckMessage`](#classLoRaMesh_1adc956ece115b35617c4973f0cfb7b313) 

Acknowledgment type packet.

#### `public String `[`receivedMsg`](#classLoRaMesh_1a285b56d8facaa17d35218bc282c0b851) 

Received message.

#### `public char `[`payload`](#classLoRaMesh_1a5a3a219bdc83318caf199968448428f6) 

Packet data.

#### `public bool `[`debugging`](#classLoRaMesh_1ae48bd2876558f5dd89779239de094573) 

Debugging flag.

#### `public std::map< unsigned short int, std::pair< unsigned short int, unsigned short int > > `[`routingTable`](#classLoRaMesh_1ab0f072af1ee6dc1e8d6680e39a35487e) 

Routing table containing nodeID, nextHop, and hopCount.

#### `public std::map< unsigned short int, unsigned short int > `[`neighbours`](#classLoRaMesh_1a6f6b22519bebac7c727140d8e43879aa) 

Neighbour table containing nodeID and hopCount.

#### `public std::vector< unsigned short int > `[`knownRoutes`](#classLoRaMesh_1a394cbd4e66044c05e06f6557fea78fd0) 

A table containing known routes by saving their nodeID.

#### `public int `[`loraSetup`](#classLoRaMesh_1a10e07d12832de958d1ec023ef01d2320)`(int myID,int ssPin,int rst,int dio0)` 

This function extends the default one from the LoRa library adding all of the necessary parameters for the [LoRaMesh](#classLoRaMesh) class and setting the necessary pins for the LoRa module to function

#### Parameters
* `myID` ID to be used by this node 

* `ssPin` The slave select pin for the LoRa module 

* `rst` The reset pin for the LoRa module 

* `dio0` The interrupt pin for the LoRa module 

#### Returns
1 if the LoRa module is successfully started, 0 otherwise

#### `public unsigned short int `[`readInt`](#classLoRaMesh_1a61e14b5fea402bb78d3865b9973a75c9)`()` 

Used for reading an integer received from the LoRa module

#### Returns
The received integer

#### `public void `[`writeInt`](#classLoRaMesh_1ab7b9e056b5901c67e8bfbdbfaf35946f)`(unsigned short int num)` 

Used for writing an integer to the LoRa module

#### Parameters
* `num` The integer to be sent

#### `public void `[`debug`](#classLoRaMesh_1a0add8dc42f41009fe123c0b8a209f2df)`(`[`packet`](#structLoRaMesh_1_1packet)` p)` 

Used for debugging the packet received from the LoRa module and printing it to the serial monitor

#### Parameters
* `p` The packet to be debugged

#### `public void `[`meshRead`](#classLoRaMesh_1a18fa94c2dd8c689ee3c53b66bc185d9b)`()` 

A function to read and interpret the packet received from the LoRa module, this is where we use the callback functions to interpret the message

#### `public `[`packet`](#structLoRaMesh_1_1packet)` `[`createMessagePacket`](#classLoRaMesh_1a875f63643e6c5140e655d105ffe32f10)`(String msg,int idToSend,int msgId,int packetType)` 

A function create a new packet

#### Parameters
* `msg` A string containing the message 

* `idToSend` The recipient ID 

* `msgId` The message ID 

* `packetType` The type of the packet being sent 

#### Returns
A packet

#### `public `[`packet`](#structLoRaMesh_1_1packet)` `[`createAckPacket`](#classLoRaMesh_1a6e4f26a6f526ea6df6f3047221ee3cd9)`(int sizeOfMsgToAck,int idToSend,int msgIdToAck)` 

A function to create an acknowledgment packet based on the message packet

#### Parameters
* `sizeOfMsgToAck` The size of the message to be acknowledged 

* `idToSend` The recipient ID 

* `msgIdToAck` The message ID to be acknowledged 

#### Returns
A packet

#### `public void `[`meshSend`](#classLoRaMesh_1af7ce1e2757faefd4a44895f157372946)`(`[`packet`](#structLoRaMesh_1_1packet)` sendPacket)` 

Function to send a packet to the LoRa module

#### Parameters
* `sendPacket` The packet to be sent

#### `public int `[`getmyID`](#classLoRaMesh_1ad34b1123675afe7f74b96ee454d79a59)`()` 

Getter for the ID of this node

#### Returns
The ID of this node

#### `public void `[`routingPacket`](#classLoRaMesh_1a3218bc895a74559a4b4f84263451f8f1)`()` 

Sends a routing packet to the LoRa module. This function constructs a packet with routing information and sends it to the LoRa module. It updates the neighbor count and checks for any neighbors with a count above a threshold. If a neighbor's count exceeds the threshold, it removes the neighbor from the routing table and neighbor list. Finally, it sends the packet to the network.

#### `public void `[`updateRoutingTable`](#classLoRaMesh_1a1b22411d4b3b0e2c6f650ba11f7c30ec)`(int id,unsigned short int size)` 

A function to update the routing table with a new node

#### Parameters
* `id` The ID of the node to be added to the routing table 

* `size` The size of the packet received

#### `public void `[`printRoutingTable`](#classLoRaMesh_1a935c7fae1f05090ac9034dd83e98ae02)`()` 

Used for printing the routing table to the serial monitor

#### `public template<>`  <br/>`void `[`debugPrint`](#classLoRaMesh_1a0db56465199a2b8e1aee7bdb20674977)`(T input)` 

A function to print debug messages to the serial monitor based on a global debugging flag

#### Parameters
* `print` The string to be printed

#### `public template<>`  <br/>`void `[`debugPrintln`](#classLoRaMesh_1afad7a1a01d44d7e79c0db6d99bc4b224)`(T input)` 

A function to print debug messages to the serial monitor with new line based on a global debugging flag

#### Parameters
* `print` The string to be printed

#### `public void `[`debugPrintln`](#classLoRaMesh_1a5ac4fe1479114fbccbf68ba1c3ad5ff2)`()` 

A function to print debug messages to the serial monitor with new line and no input, aka. empty line based on a global debugging flag

#### `public void `[`readMsgCallback`](#classLoRaMesh_1a72625ac94eecb0ff02cde7328882412d)`(std::function< void(int, int, String)> callbackData)` 

Register a callback function to be called when a message is received so that any user can interpret the message as they want

#### Parameters
* `callbackData` The callback function to register.

#### `public void `[`readAckCallback`](#classLoRaMesh_1a1c3bc1adb648a21d65ba6316f31b1f3a)`(std::function< void(int, int)> callbackAck)` 

Register a callback function to be called when a acknowlegment is received so that any user can interpret the acknowlegment as they want

#### Parameters
* `callbackAck` The callback function to register.

# struct `LoRaMesh::packet` 

Structure representing a packet in the [LoRaMesh](#classLoRaMesh) network.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public unsigned short int `[`source`](#structLoRaMesh_1_1packet_1aed8cdb7dd4dda94f18340a395ba0afd6) | Sender nodeID.
`public unsigned short int `[`destination`](#structLoRaMesh_1_1packet_1a1ccdeaf09641c6d5e73c7a84cc6fbace) | Receiver nodeID.
`public unsigned short int `[`nextHop`](#structLoRaMesh_1_1packet_1afbfc46553f71978e877d0e1a6df4f1e5) | Next hop nodeID.
`public unsigned short int `[`packetSize`](#structLoRaMesh_1_1packet_1a317674f198333551f06ec5de1e225cff) | Size of the packet.
`public unsigned short int `[`msgId`](#structLoRaMesh_1_1packet_1a606824c9894b8efc39bbab4f9f74876e) | Message ID.
`public byte `[`type`](#structLoRaMesh_1_1packet_1a963d64824e9f8ab8cd279d39b541dfb4) | Type of the packet.
`public byte `[`preamble`](#structLoRaMesh_1_1packet_1a13071c050d5df11b3a3d54f5f6d8efa2) | Preamble.
`public char `[`payload`](#structLoRaMesh_1_1packet_1a977a45b254e4f7e051e5f6680ba0eb7e) | Packet data.

## Members

#### `public unsigned short int `[`source`](#structLoRaMesh_1_1packet_1aed8cdb7dd4dda94f18340a395ba0afd6) 

Sender nodeID.

#### `public unsigned short int `[`destination`](#structLoRaMesh_1_1packet_1a1ccdeaf09641c6d5e73c7a84cc6fbace) 

Receiver nodeID.

#### `public unsigned short int `[`nextHop`](#structLoRaMesh_1_1packet_1afbfc46553f71978e877d0e1a6df4f1e5) 

Next hop nodeID.

#### `public unsigned short int `[`packetSize`](#structLoRaMesh_1_1packet_1a317674f198333551f06ec5de1e225cff) 

Size of the packet.

#### `public unsigned short int `[`msgId`](#structLoRaMesh_1_1packet_1a606824c9894b8efc39bbab4f9f74876e) 

Message ID.

#### `public byte `[`type`](#structLoRaMesh_1_1packet_1a963d64824e9f8ab8cd279d39b541dfb4) 

Type of the packet.

#### `public byte `[`preamble`](#structLoRaMesh_1_1packet_1a13071c050d5df11b3a3d54f5f6d8efa2) 

Preamble.

#### `public char `[`payload`](#structLoRaMesh_1_1packet_1a977a45b254e4f7e051e5f6680ba0eb7e) 

Packet data.

Generated by [Moxygen](https://sourcey.com/moxygen)