#include "storage.h"
#include <Arduino.h>
#include <FlashStorage.h>
#include "config.h"

struct FlashSettings
{
    uint8_t magic;
    uint8_t bpm;
};
FlashStorage(flashSettingsStorage, FlashSettings);

static uint8_t g_bpm = 0;
static bool g_dirty = false;
static uint32_t g_lastChangeMs = 0;

void storageInit(uint8_t defaultBpm)
{
    FlashSettings s = flashSettingsStorage.read();
    if (s.magic == config::EEPROM_MAGIC)
    {
        g_bpm = (uint8_t)constrain((int)s.bpm, (int)config::BPM_MIN, (int)config::BPM_MAX);
    }
    else
    {
        g_bpm = defaultBpm;
    }
}

uint8_t storageGetBpm()
{
    return g_bpm;
}

void storageScheduleSave(uint8_t bpm)
{
    g_bpm = (uint8_t)constrain((int)bpm, (int)config::BPM_MIN, (int)config::BPM_MAX);
    g_dirty = true;
    g_lastChangeMs = millis();
}

void storageLoop(uint32_t now)
{
    if (g_dirty && (now - g_lastChangeMs) >= config::BPM_SAVE_DELAY_MS)
    {
        FlashSettings s;
        s.magic = config::EEPROM_MAGIC;
        s.bpm = g_bpm;
        flashSettingsStorage.write(s);
        g_dirty = false;
    }
}