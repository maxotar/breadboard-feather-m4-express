#include "display.h"
#include <Adafruit_LEDBackpack.h>
#include <Arduino.h>
#include "config.h"

static Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void displayInit()
{
    alpha4.begin(0x70);
}

void displayBpm(int value)
{
    char s[5];
    snprintf(s, sizeof(s), "%4d", value);
    for (int i = 0; i < 4; ++i)
        alpha4.writeDigitAscii(i, s[i]);
    alpha4.writeDisplay();
}