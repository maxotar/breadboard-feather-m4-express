#pragma once
#include <stdint.h>

class Button
{
public:
    Button();
    void init(uint8_t pin);
    // Call regularly with current millis()
    void update(uint32_t now);

    // Event queries (consume the event)
    bool wasPressed();   // falling edge
    bool wasReleased();  // rising edge
    bool shouldRepeat(); // true when a repeat action should be performed (long-press repeat)

    // State queries
    bool isPressed() const; // debounced state == LOW

private:
    uint8_t pin_;
    uint8_t lastRaw_;
    uint8_t debounced_;
    uint8_t prevDebounced_;
    uint32_t lastDebounceTime_;
    uint32_t pressStartMs_;
    bool longPressActive_;
    uint32_t lastRepeatMs_;

    // Pending one-shot flags set during update() and consumed by the callers
    bool pressedFlag_;
    bool releasedFlag_;
};