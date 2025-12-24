#include "neopixel.h"
#include <Adafruit_NeoPixel.h>
#include "config.h"

static Adafruit_NeoPixel pixels(config::NEO_COUNT, config::NEO_PIN, NEO_GRB + NEO_KHZ800);

void neoInit()
{
    pixels.begin();
    pixels.setBrightness(config::NEO_BRIGHTNESS);
}

void neoSetBlue(bool on)
{
    if (on)
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));
    else
        pixels.clear();
    pixels.show();
}