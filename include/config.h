#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <array>

namespace config
{
    // NeoPixel
    inline constexpr uint8_t NEO_PIN = 8;
    inline constexpr uint8_t NEO_COUNT = 1;
    inline constexpr uint8_t NEO_BRIGHTNESS = 50; // 0-255 dimmed

    // BPM range & steps
    inline constexpr uint8_t BPM_MIN = 0;
    inline constexpr uint8_t BPM_MAX = 155;
    inline constexpr uint8_t BPM_STEP = 5;

    // Timing (ms)
    inline constexpr uint32_t DEBOUNCE_MS = 50u;
    inline constexpr uint32_t LONG_PRESS_MS = 500u;
    inline constexpr uint32_t REPEAT_INTERVAL_MS = 150u;
    inline constexpr uint32_t PULSE_MAX_MS = 150u;

    // Built-in indicator
    inline constexpr uint8_t INDICATOR_PIN = 13;

    // Persistence
    inline constexpr uint8_t EEPROM_MAGIC = 0xC5;
    inline constexpr uint32_t BPM_SAVE_DELAY_MS = 2000u;

    // Buttons
    inline constexpr uint8_t BUTTON_COUNT = 3;
    inline constexpr std::array<uint8_t, BUTTON_COUNT> BUTTON_PINS = {(uint8_t)A2, (uint8_t)A3, (uint8_t)A4};

    // LED indicators
    inline constexpr uint8_t LED_COUNT = 5;
    inline constexpr std::array<uint8_t, LED_COUNT> LED_PINS = {11, 10, 9, 6, 5};
}
