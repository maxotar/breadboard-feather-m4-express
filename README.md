# Breadboard Feather M4 Metronome 🔧🎵

Short, focused firmware for an Adafruit Feather M4 Express-based metronome.

## Features ✅
- Onboard NeoPixel pulse synced to BPM (dimmed blue)
- Haptic motor vibration synced to BPM via DRV2605L driver (effect #1)
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
  - `haptic.*` — DRV2605L haptic motor driver wrapper
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

---

## Hardware

- Board: Adafruit Feather M4 Express (ATSAMD51 / 3.3V logic). Power the project from the Feather 3.3V rail. Ensure a common ground between all modules.
- Display: 4 × 14‑segment alphanumeric backpacks (HT16K33 I2C backpacks). All four are driven via I2C (default address 0x70 in this project). Connect SDA/SCL to the Feather's I2C pins (the SDA / SCL labeled pins on the Feather). Power the displays from 3.3V.
- Haptic Motor: Adafruit DRV2605L Haptic Motor Driver Breakout. Connect via I2C (SDA/SCL to the Feather's I2C pins, default address 0x5A). Power from 3.3V. The motor triggers effect #1 (strong click) synchronized with each beat. Ensure a vibrating motor (ERM or LRA) is connected to the DRV2605L driver outputs.
- Onboard NeoPixel: the Feather's built‑in NeoPixel is used for the beat pulse (configured in code). No external resistor required for the built‑in pixel; for external NeoPixel strips use a ~470Ω data resistor and a 1000µF power capacitor across V+ and GND.
- Status LEDs (5): pins (MSB → LSB) 11, 10, 9, 6, 5. Wire each LED with a series current‑limiting resistor (recommended 220–470Ω) from the MCU pin to the LED to GND.
- Buttons: A2 = Toggle (start/stop), A3 = Down (-5 BPM), A4 = Up (+5 BPM). Buttons are wired to the pin and to GND; the firmware uses INPUT_PULLUP, so pressing pulls the pin LOW.
- Built‑in indicator: pin 13 (onboard LED). Driven HIGH when metronome is enabled.
- Resistors & general wiring:
  - External indicator LEDs: 220–470Ω series resistors per LED.
  - Buttons: no external pull‑downs required (using internal pull‑ups). If you prefer external components, use a 10kΩ pull‑up and wire button between pin and GND.
  - NeoPixel/LED power: if using external LEDs or strips, ensure adequate power supply current and common ground.
- Notes:
  - All logic is 3.3V; check peripheral voltage compatibility before wiring 5V devices.
  - Keep I2C wiring short and share common GND. If adding multiple I2C backpacks, ensure unique addresses or use the same HT16K33 addressing scheme used by the project.