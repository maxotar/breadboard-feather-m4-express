#include "metronome.h"
#include "neopixel.h"
#include "config.h"
#include <Arduino.h>

static bool g_enabled = true;
static uint8_t g_bpm = 80;
static bool g_pulseActive = false;
static uint32_t g_nextBeatMs = 0;
static uint32_t g_beatEndMs = 0;
static uint32_t g_beatIntervalMs = 0;
static uint32_t g_pulseLengthMs = 0;

static void updateTimingForBpm()
{
    g_beatIntervalMs = 60000u / (uint32_t)max(1, (int)g_bpm);
    g_pulseLengthMs = (uint32_t)min(g_beatIntervalMs / 6u, config::PULSE_MAX_MS);
}

void metronomeInit()
{
    updateTimingForBpm();
    if (g_bpm > 0)
        g_nextBeatMs = millis();
}

void metronomeSetBpm(uint8_t bpm)
{
    g_bpm = constrain((int)bpm, (int)config::BPM_MIN, (int)config::BPM_MAX);
    updateTimingForBpm();
}

void metronomeEnable(bool enable)
{
    g_enabled = enable;
    if (!g_enabled)
    {
        // Ensure pulse stops
        if (g_pulseActive)
        {
            neoSetBlue(false);
            g_pulseActive = false;
        }
    }
    else
    {
        if (g_bpm > 0)
            g_nextBeatMs = millis();
    }
}

void metronomeScheduleImmediateBeat()
{
    g_nextBeatMs = millis();
}

void metronomeLoop(uint32_t now)
{
    if (!g_enabled || g_bpm == 0)
    {
        if (g_pulseActive)
        {
            neoSetBlue(false);
            g_pulseActive = false;
        }
        return;
    }

    if (!g_pulseActive)
    {
        if (now >= g_nextBeatMs)
        {
            // Start pulse
            neoSetBlue(true);
            g_pulseActive = true;
            g_beatEndMs = now + g_pulseLengthMs;
            g_nextBeatMs = now + g_beatIntervalMs;
        }
    }
    else
    {
        if (now >= g_beatEndMs)
        {
            neoSetBlue(false);
            g_pulseActive = false;
        }
    }
}

bool metronomeIsPulseActive()
{
    return g_pulseActive;
}