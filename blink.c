/*++

Morse Encoder Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

Author:

    Axel Rietschin - June 27, 2020

Environment:

    ESP32 with ESP-IDF 4.x

--*/

#include <stdio.h>
#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "MorseEncoder.h"

// Can use project configuration menu (idf.py menuconfig) to choose
// the GPIO to blink and the base time unit, or you can edit the
// following lines and define values here.
//
// A good base time unit is 100ms. The pin depends on the hardware
// configuration. The Huzzah32 ESP32 board has the LED wired on pin 13.
//

#define BASE_TIME_UNIT_MS   CONFIG_BASE_TIME_UNIT_MS // or use 100
#define BLINK_GPIO          CONFIG_BLINK_GPIO        // or use your pin #

//
// Hardware-dependent function supplied to the encoder
//

inline int
MorseEncoder_GetBaseTimeUnitMs()
{
    return BASE_TIME_UNIT_MS;
}

void
MorseEncoder_SetSignalState(int state, int pauseDurationMs)
{
    gpio_set_level(BLINK_GPIO, state ? 1 : 0);

    if (pauseDurationMs > 0)
    {
        vTaskDelay(pauseDurationMs / portTICK_PERIOD_MS);
    }
}

//
// Sample program
//

void
Initialize()
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BLINK_GPIO, 0);
}

void
app_main()
{
    Initialize();

    while (1)
    {
        MorseEncoder_TransmitStartOfMessageSymbol();
        MorseEncoder_TransmitString("MY NAME IS AXEL. I JUST WROTE A MORSE ENCODER :-)");
        MorseEncoder_TransmitEndOfMessageSymbol();
    }
}
