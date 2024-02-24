#include <Arduino.h>
#include <LoRaMesh.h>

extern LoRaMesh loramesh;

void prepareAndSendMessage(String incoming) { // Prepare and send message
  int sendID = 0;
  int recvID = 0;
  int msgID = 0;
  String message = "";

  int i;
  char delimiter[] = ":";
  char *p;
  char string[300];
  String test = incoming;
  String words[5];

  test.toCharArray(string, sizeof(string));
  i = 0;
  p = strtok(string, delimiter);
  while(p && i < 5)
  {
    words[i] = p;
    p = strtok(NULL, delimiter);
    ++i;
  }
  debugPrintln(words[0]);
  sendID = words[1].toInt();
  recvID = words[2].toInt();
  msgID = words[3].toInt();
  message = words[4];
  debugPrintln(sendID);
  debugPrintln(recvID);
  debugPrintln(msgID);
  debugPrintln(message);
  loramesh.meshSend(loramesh.createMessagePacket(message, recvID, msgID));
}

//Help menu for the user
String help = 
    "Commands:\n"
    "For help, type h\n"
    "To get the routing table, type p\n"
    "To send a message, type the following command in this format: w:{sendID}:{recvID}:{msgID}:{msgData}\n";

void menu(String incoming) { // Print help
    debugPrint("I received: ");
    debugPrintln(incoming);
    if (incoming[0] == 'p') loramesh.printRoutingTable();
    else if (incoming[0] == 'w') { prepareAndSendMessage(incoming); Serial.println("ok"); }
    else if (incoming[0] == 'h') Serial.println(help);
    else Serial.println("Invalid command, for help type h");
}

// Check if string is numeric
boolean isNumeric(String str) {
    unsigned int stringLength = str.length();
 
    if (stringLength == 0) {
        return false;
    }
 
    boolean seenDecimal = false;
 
    for(unsigned int i = 0; i < stringLength; ++i) {
        if (isDigit(str.charAt(i))) {
            continue;
        }
 
        if (str.charAt(i) == '.') {
            if (seenDecimal) {
                return false;
            }
            seenDecimal = true;
            continue;
        }
        return false;
    }
    return true;
}