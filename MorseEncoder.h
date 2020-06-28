/* Morse Encoder Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

    Author:

        Axel Rietschin - June 27, 2020
        
    Environment:
    
        Any

*/

#pragma once

//
// Environment-dependent functions supplied by the user
//

extern inline int
MorseEncoder_GetBaseTimeUnitMs();

extern void
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
