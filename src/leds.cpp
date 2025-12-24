#include "leds.h"
#include <Arduino.h>
#include "config.h"
#include "utils.h"

void ledsInit()
{
    for (uint8_t i = 0; i < config::LED_COUNT; ++i)
    {
        pinMode(config::LED_PINS[i], OUTPUT);
        digitalWrite(config::LED_PINS[i], LOW);
    }
}

void ledsUpdateBpm(uint8_t bpm)
{
    int scaled = scaleBpmToBits(bpm, (uint8_t)config::BPM_MAX, (uint8_t)config::LED_COUNT);

    for (uint8_t i = 0; i < config::LED_COUNT; ++i)
    {
        uint8_t pin = config::LED_PINS[i];
        int bit = (scaled >> (config::LED_COUNT - 1 - i)) & 0x1; // MSB on LED_PINS[0]
        digitalWrite(pin, bit ? HIGH : LOW);
    }
}