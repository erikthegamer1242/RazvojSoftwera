#include <Arduino.h>
extern bool debugging;

template <typename T> void debugPrint(T print) {
    if (debugging) Serial.print(print);
}

template <typename T> void debugPrintln(T print) {
    if (debugging) Serial.println(print);
}

void debugPrintln() {
    if (debugging) Serial.println();
}