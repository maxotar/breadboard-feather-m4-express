# Breadboard Feather M4 Metronome 🔧🎵

Short, focused firmware for an Adafruit Feather M4 Express-based metronome.

## Features ✅
- Onboard NeoPixel pulse synced to BPM (dimmed blue)
- 4‑digit 14‑segment display shows current BPM
- Buttons: toggle start/stop, -5 BPM, +5 BPM; long-press repeat supported
- 5 external LEDs show scaled BPM in 5‑bit binary (MSB on pin 11)
- Persistent BPM saved to flash (FlashStorage) with delayed write to reduce wear
- Non-blocking, millis()-based scheduler (no delays)

---

## Project layout 🔎
- `src/` — implementation files
  - `main.cpp` — orchestrator (wires all modules)
  - `button.*` — debounce / long-press logic
  - `neopixel.*` — NeoPixel helper
  - `display.*` — 14‑segment display wrapper
  - `leds.*` — LED indicator wrapper
  - `storage.*` — FlashStorage wrapper (save/load + delayed writes)
  - `metronome.*` — beat scheduling and pulse control
- `include/`
  - `config.h` — pin and timing constants
  - `utils.h` — small pure helpers (scaling)
- `platformio.ini` — build & dependency configuration

---

## Build & upload 🚀
Requires PlatformIO (tested on Windows).

- Build: `pio run`
- Upload: `pio run --target upload`

If you add C++17-only constructs (e.g., `inline constexpr` globals) you may need to add `build_flags = -std=gnu++17` to `platformio.ini`.

---

## Testing & CI 💡
- No tests yet. Recommended first tests: `utils.h` (pure helper) and `button` timing logic.