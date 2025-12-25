#include "haptic.h"
#include <Adafruit_DRV2605.h>
#include <Arduino.h>

static Adafruit_DRV2605 drv;

void hapticInit()
{
    if (!drv.begin())
    {
        Serial.println("DRV2605 not found");
        return;
    }

    Serial.println("DRV2605 found");
    drv.selectLibrary(1);
    drv.setMode(DRV2605_MODE_INTTRIG);
}

void hapticTriggerBeat()
{
    // Use effect #1 (strong click - 100%)
    drv.setWaveform(0, 1);  // effect #1
    drv.setWaveform(1, 0);  // end of waveform
    drv.go();
}
