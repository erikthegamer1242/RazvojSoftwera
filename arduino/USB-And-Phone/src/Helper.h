#include <Arduino.h>
#include <LoRaMesh.h>

extern LoRaMesh loramesh;

/**
 * This function takes the incoming data as String,
 * oragnizes it, and sends a message to the LoRaMesh network.
 *
 * @param[in] incoming Incoming data to be sent
 */
void prepareAndSendMessage(String incoming) {
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

/**
 * String that contains the help message for the user
 * to understand the commands that can be used.
 *
 */
String help = 
    "Commands:\n"
    "For help, type h\n"
    "To get the routing table, type p\n"
    "To send a message, type the following command in this format: w:{sendID}:{recvID}:{msgID}:{msgData}\n";

/**
 * A function that takes incoming data from the user menu and 
 * checks if it is a valid command, and if it is, it will
 * execute the command.
 *
 * @param[in] incoming Incoming data from the user menu
 */
void menu(String incoming) { 
    debugPrint("I received: ");
    debugPrintln(incoming);
    if (incoming[0] == 'p') loramesh.printRoutingTable();
    else if (incoming[0] == 'w') { prepareAndSendMessage(incoming); Serial.println("ok"); }
    else if (incoming[0] == 'h') Serial.println(help);
    else Serial.println("Invalid command, for help type h");
}

/**
 * This function iterates through a string and checks if
 * the string contains all numbers.
 *
 * @param[in] str String to be checked
 * @return True if the string contains all numbers, false otherwise
 */
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