/* Morse Encoder Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

    Author:

        Axel Rietschin - June 27, 2020.
        
    Environment:
    
        ESP32 with ESP-IDF 4.x

*/

#pragma once

// Can use project configuration menu (idf.py menuconfig) to choose
// the GPIO to blink and the base time unit, or you can edit the
// following lines and set values here.
//
// A good base time unit is 100ms. The pin depends on the hardware
// configuration. The Huzzah32 ESP32 board has the LED wired on pin 13.
//

#include "sdkconfig.h"

#define BASE_TIME_UNIT_MS   CONFIG_BASE_TIME_UNIT_MS
#define BLINK_GPIO          CONFIG_BLINK_GPIO

//
// Hardware-dependent functions supplied by the user
//

void
MorseEncoder_SetSignalState(int state, int pauseDurationMs);

//
// API functions
//

void
MorseEncoder_TransmitStartOfMessageSymbol();

int
MorseEncoder_TransmitSymbol(int c);

void
MorseEncoder_TransmitString(const char* string);

void
MorseEncoder_TransmitEndOfMessageSymbol();

void
MorseEncoder_TransmitErrorSymbol();

void
MorseEncoder_TransmitUnderstoodSymbol();

void
MorseEncoder_TransmitWaitSymbol();

void
MorseEncoder_TransmitClearToSendSymbol();
