/* Morse Encoder

   This code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
   
   Description:

        ITU R-REC-M.1677-1-200910 International Morse Encoder, as described in
        https://www.itu.int/dms_pubrec/itu-r/rec/m/R-REC-M.1677-1-200910-I!!PDF-E.pdf

    Author:

        Axel Rietschin - June 27, 2020.
        
    Environment:
    
        ESP32 with ESP-IDF 4.x

*/

#include <ctype.h>

#include "MorseEncoder.h"

//
// Internal functions
//

static void
DotSymbol()
{
    MorseEncoder_SetSignalState(1, MorseEncoder_GetBaseTimeUnitMs());
    MorseEncoder_SetSignalState(0, MorseEncoder_GetBaseTimeUnitMs());
}

static void
DashSymbol()
{
    MorseEncoder_SetSignalState(1, 3 * MorseEncoder_GetBaseTimeUnitMs());
    MorseEncoder_SetSignalState(0, MorseEncoder_GetBaseTimeUnitMs());
}

static void
EndOfSymbol()
{
    MorseEncoder_SetSignalState(0, 2 * MorseEncoder_GetBaseTimeUnitMs());  // 3 total
}

static void
EndOfWord()
{
    MorseEncoder_SetSignalState(0, 6 * MorseEncoder_GetBaseTimeUnitMs());  // 7 total
}

static void
EndOfSentence()
{
    MorseEncoder_SetSignalState(0, 10 * MorseEncoder_GetBaseTimeUnitMs());  // 11 total
}

static void
TransmitSymbol(const char* morseSymbol)
{
    if (morseSymbol)
    {
        while (*morseSymbol)
        {
            if (*morseSymbol++ == '.')
            {
                DotSymbol();
            }
            else
            {
                DashSymbol();
            }
        }
        
        EndOfSymbol();
    }
}

//
// API functions
//

int
MorseEncoder_TransmitSymbol(int c)
{
    const char* symbol = NULL;

    static const char* MorseCode[] =
    {
        ".-",       // A
        "-...",     // B
        "-.-.",     // C
        "-..",      // D
        ".",        // E
        "..-.",     // F
        "--.",      // G
        "....",     // H
        "..",       // I
        ".---",     // J
        "-.-",      // K
        ".-..",     // L
        "--",       // M
        "-.",       // N
        "---",      // O
        ".--.",     // P
        "--.-",     // Q
        ".-.",      // R
        "...",      // S
        "-",        // T
        "..-",      // U
        "...-",     // V
        ".--",      // W
        "-..-",     // X
        "-.--",     // Y
        "--.."      // Z
        "-----",    // 0
        ".----",    // 1
        "..---",    // 2
        "...--",    // 3
        "....-",    // 4
        ".....",    // 5
        "-....",    // 6
        "--...",    // 7
        "---..",    // 8
        "----."     // 9
    };

    if (c >= '0' && c <= '9')
    {
        symbol = MorseCode[c + 'z' - '0'];
    }
    else
    {
        c = tolower(c);

        if (c >= 'a' && c <= 'z')
        {
            symbol = MorseCode[c - 'a'];
        }
        else
        {
            switch (c)
            {
                case '.' : symbol = ".-.-.-"; break;    // Full stop (period)
                case ',' : symbol = "--..--"; break;    // Comma
                case ':' : symbol = "---..."; break;    // Colon or division sign
                case '?' : symbol = "..--.."; break;    // Question mark (note of interrogation or request for repetition of a transmission not understood)
                case '\'': symbol = ".----."; break;    // Apostrophe
                case '-' : symbol = "-....-"; break;    // Hyphen or dash or subtraction sign
                case '/' : symbol = "-..-.";  break;    // Fraction bar or division sign
                case '(' : symbol = "-.--.";  break;    // Left-hand bracket (parenthesis)
                case ')' : symbol = "-.--.-"; break;    // Right-hand bracket (parenthesis)
                case '"' : symbol = ".-..-."; break;    // Inverted commas (quotation marks) (before and after the words)
                case '=' : symbol = "-...-";  break;    // Double hyphen
                case '+' : symbol = ".-.-.";  break;    // Cross or addition sign
                case '*' : symbol = "-..-";   break;    // Multiplication
                case '@' : symbol = ".--.-."; break;    // Commercial At
            }
        }
    }

    if (!symbol)
    {
        return 0;
    }
    
    TransmitSymbol(symbol);
    
    return 1;
}

void
MorseEncoder_TransmitString(const char* string)
{
    if (string)
    {
        int sentenceOpen = 0;
        
        while (*string)
        {
            if (*string == ' ')
            {
                while (* ++string == ' ') 
                {
                    ;
                }
                
                if (*string && sentenceOpen)
                {
                    EndOfWord();
                }
            }
            else
            {
                if (MorseEncoder_TransmitSymbol(*string))
                {
                    if (*string == '.')
                    {
                        EndOfSentence();

                        if (sentenceOpen)
                        {
                            sentenceOpen = 0;
                        }
                    }
                    else if (!sentenceOpen)
                    {
                        sentenceOpen = 1;
                    }
                }

                ++string;
            }
        }

        if (sentenceOpen)
        {
            EndOfSentence();
        }
    }
}

void
MorseEncoder_TransmitStartOfMessageSymbol()
{
    TransmitSymbol("-.-.-");
    EndOfSentence();
}

void
MorseEncoder_TransmitEndOfMessageSymbol()
{
    TransmitSymbol("...-.-");
    MorseEncoder_SetSignalState(0, 21 * MorseEncoder_GetBaseTimeUnitMs());  // 22 total
}

void
MorseEncoder_TransmitErrorSymbol()
{
    TransmitSymbol("........");
    EndOfSentence();
}

void
MorseEncoder_TransmitUnderstoodSymbol()
{
    TransmitSymbol("...-.");
    EndOfSentence();
}

void
MorseEncoder_TransmitWaitSymbol()
{
    TransmitSymbol(".-...");
    EndOfSentence();
}

void
MorseEncoder_TransmitClearToSendSymbol()
{
    TransmitSymbol("-.-");
    EndOfSentence();
}
