#include <Arduino.h>
#include "neopixel.h"
#include "storage.h"
#include "display.h"
#include "leds.h"
#include "button.h"
#include "metronome.h"
#include "config.h"

enum ButtonIndex
{
  BTN_TOGGLE = 0,
  BTN_DOWN = 1,
  BTN_UP = 2
};

// --- State ---
uint8_t bpm = 80;             // current BPM (initialized to 80)
bool metronomeEnabled = true; // start/stop toggle

// Button instances
Button buttons[config::BUTTON_COUNT];

// Update built-in indicator LED to reflect metronome enabled state (ON when enabled)
void updateIndicatorState()
{
  digitalWrite(config::INDICATOR_PIN, metronomeEnabled ? HIGH : LOW);
}

void changeBpm(int newBpm)
{
  int newVal = constrain(newBpm, (int)config::BPM_MIN, (int)config::BPM_MAX);
  if (newVal == bpm)
    return;

  bpm = newVal;
  displayBpm(bpm);
  // Update metronome timing and LEDs
  metronomeSetBpm(bpm);
  ledsUpdateBpm(bpm);

  // schedule delayed save via storage module
  storageScheduleSave(bpm);

  if (metronomeEnabled && bpm > 0)
  {
    metronomeScheduleImmediateBeat();
  }
  else if (bpm == 0)
  {
    // Ensure pulse stops immediately if we set bpm to 0
    metronomeLoop(millis());
  }
}

void handleButtonPress(ButtonIndex idx)
{
  if (idx == BTN_TOGGLE)
  {
    metronomeEnabled = !metronomeEnabled;
    updateIndicatorState();
    metronomeEnable(metronomeEnabled);
    if (metronomeEnabled && bpm > 0)
      metronomeScheduleImmediateBeat();
  }
  else if (idx == BTN_DOWN)
  {
    changeBpm((int)bpm - (int)config::BPM_STEP);
  }
  else if (idx == BTN_UP)
  {
    changeBpm((int)bpm + (int)config::BPM_STEP);
  }
}

// --- Setup & Loop ---
void setup()
{
  Serial.begin(9600);
  displayInit();

  // NeoPixel
  neoInit();
  neoSetBlue(false);

  // Buttons: initialize Button objects
  for (uint8_t i = 0; i < config::BUTTON_COUNT; ++i)
  {
    buttons[i].init(config::BUTTON_PINS[i]);
  }

  // Indicator (built-in LED)
  pinMode(config::INDICATOR_PIN, OUTPUT);
  updateIndicatorState();

  // LED pins
  ledsInit();

  // Initialize storage and load persisted BPM if present
  storageInit(bpm);
  bpm = storageGetBpm();

  displayBpm(bpm);
  metronomeSetBpm(bpm);
  ledsUpdateBpm(bpm);
  if (bpm > 0)
    metronomeScheduleImmediateBeat();
}

void loop()
{
  uint32_t now = (uint32_t)millis();

  // Update buttons and process presses
  for (uint8_t i = 0; i < config::BUTTON_COUNT; ++i)
  {
    buttons[i].update(now);
    if (buttons[i].wasPressed())
    {
      handleButtonPress((ButtonIndex)i);
    }
  }

  // Handle long-press repeating for up/down buttons (constant rate)
  for (uint8_t i = 0; i < config::BUTTON_COUNT; ++i)
  {
    if ((i == BTN_DOWN || i == BTN_UP) && buttons[i].shouldRepeat())
    {
      if (i == BTN_DOWN)
        changeBpm((int)bpm - config::BPM_STEP);
      else
        changeBpm((int)bpm + config::BPM_STEP);
    }
  }

  // Storage background tasks (delayed saves)
  storageLoop(now);

  // Metronome background processing
  metronomeLoop(now);
}
