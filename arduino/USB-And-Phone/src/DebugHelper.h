#include <Arduino.h>
extern bool debugging;

/**
 * @brief Helper header file to provide debugging functionality.
 * 
 * This file provides a set of functions to print debug messages to the serial monitor
 * based on a global debugging flag from the main cpp file.
 */


/**
 * A function to print debug messages to the serial monitor
 * based on a global debugging flag
 * 
 * @param [in] print The string to be printed
 * 
 */
template <typename T>
void debugPrint(T print)
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
void debugPrintln(T print)
{
    if (debugging)
        Serial.println(print);
}

/**
 * A function to print debug messages to the serial monitor with new line
 * and no input, aka. empty line based on a global debugging flag
 * 
 */
void debugPrintln()
{
    if (debugging)
        Serial.println();
}