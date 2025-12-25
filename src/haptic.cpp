#include "haptic.h"
#include <Adafruit_DRV2605.h>
#include <Arduino.h>

static Adafruit_DRV2605 drv;
static bool g_initialized = false;

void hapticInit()
{
    if (!drv.begin())
    {
        Serial.println("DRV2605 not found");
        g_initialized = false;
        return;
    }

    Serial.println("DRV2605 found");
    drv.selectLibrary(1);
    drv.setMode(DRV2605_MODE_INTTRIG);
    g_initialized = true;
}

void hapticTriggerBeat()
{
    if (!g_initialized)
        return;

    // Use effect #1 (strong click - 100%)
    drv.setWaveform(0, 1);  // effect #1
    drv.setWaveform(1, 0);  // end of waveform
    drv.go();
}
