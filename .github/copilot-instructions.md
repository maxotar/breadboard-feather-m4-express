# Breadboard Feather M4 Metronome - AI Agent Guide

## Architecture Overview
This is an Arduino firmware for an Adafruit Feather M4 Express metronome with multi-peripheral coordination. The architecture is a **non-blocking event loop** with modular hardware wrappers and a centralized orchestrator in [src/main.cpp](../src/main.cpp).

**Core modules** (each with `.h/.cpp`):
- `button` — debouncing + long-press/repeat logic (INPUT_PULLUP, active LOW)
- `metronome` — timing engine using `millis()`, coordinates pulse start/end without blocking
- `neopixel`, `haptic`, `display`, `leds` — hardware wrappers (NeoPixel, DRV2605L I2C haptic, HT16K33 I2C 14-segment display, GPIO LEDs)
- `storage` — delayed flash writes (FlashStorage library) to minimize wear; saves BPM only after 2s of inactivity

**Data flow**: User button press → `main.cpp` detects via `Button::wasPressed()` → calls `changeBpm()` → updates all peripherals (display, LEDs, metronome timing) → schedules delayed flash save → metronome independently pulses NeoPixel/haptic on beat intervals.

## Critical Conventions

### Timing & Non-Blocking Pattern
- **Never use `delay()`**. All timing is `millis()`-based with explicit state machines.
- Pattern: store `nextEventMs` timestamp, compare `now >= nextEventMs` in loop.
- Example: [src/metronome.cpp](../src/metronome.cpp) tracks `g_nextBeatMs` and `g_beatEndMs` for pulse on/off scheduling.

### Configuration & Constants
- All hardware pins, timing constants, and magic numbers live in [include/config.h](../include/config.h) as `inline constexpr` (C++17).
- Use `config::` namespace prefix: `config::DEBOUNCE_MS`, `config::BUTTON_PINS`, etc.
- Button pins are `INPUT_PULLUP` wired to GND (active LOW).

### Button Handling
- Buttons use custom `Button` class ([src/button.cpp](../src/button.cpp)) with debouncing, long-press detection, and auto-repeat.
- `wasPressed()` is edge-triggered, cleared after read.
- `shouldRepeat()` returns true at `REPEAT_INTERVAL_MS` intervals when held past `LONG_PRESS_MS`.
- Main loop polls all buttons each cycle, no interrupts.

### Flash Storage Wear Leveling
- BPM changes call `storageScheduleSave()` which sets dirty flag and timestamp.
- Actual flash write deferred 2s (`BPM_SAVE_DELAY_MS`) in `storageLoop()` to coalesce rapid changes.
- Magic byte (`EEPROM_MAGIC = 0xC5`) validates saved data on boot.

### Peripheral Coordination
- I2C devices share SDA/SCL (display at 0x70, haptic at 0x5A).
- All peripherals initialized in `setup()` before loading saved state.
- When BPM changes, update sequence: display → metronome timing → LEDs → storage schedule → optionally trigger immediate beat.

## Build & Debug Workflow

**Build/Upload** (PlatformIO):
```
pio run                  # compile
pio run --target upload  # flash to board
```

**Dependencies** (auto-resolved by PlatformIO, defined in `platformio.ini`):
- Adafruit NeoPixel, LED Backpack Library, DRV2605 Library
- FlashStorage (cmaglie)

**Common tasks**:
- Serial monitor: `pio device monitor -b 9600`
- Clean build: `pio run --target clean`
- No unit tests configured yet; recommended first tests: `utils.h::scaleBpmToBits()` and `Button` state transitions.

## Key Files for Changes

- **Adding new button**: update `config::BUTTON_COUNT` / `BUTTON_PINS`, extend `ButtonIndex` enum, add handler in `handleButtonPress()`.
- **Changing timing constants**: edit [include/config.h](../include/config.h) (e.g., `LONG_PRESS_MS`, `REPEAT_INTERVAL_MS`, `PULSE_MAX_MS`).
- **New peripheral**: create wrapper in `src/`, initialize in `setup()`, update state in appropriate handler (e.g., `changeBpm()` for BPM-dependent output).
- **BPM range/scaling**: adjust `BPM_MIN`/`BPM_MAX` in config.h, ensure LED scaling in `leds.cpp` uses `utils.h::scaleBpmToBits()`.

## Hardware Constraints
- **3.3V logic only** (ATSAMD51). Check peripheral voltage compatibility.
- **I2C addressing**: display defaults to 0x70, haptic to 0x5A. Multi-backpack displays must have unique addresses or be chained.
- **NeoPixel**: built-in pixel on pin 8. External strips need 470Ω data resistor + 1000µF cap.
- **Button wiring**: INPUT_PULLUP + wire to GND. No external pull-downs needed.
