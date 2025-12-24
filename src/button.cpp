#include "button.h"
#include <Arduino.h>
#include "config.h"

Button::Button()
    : pin_(255), lastRaw_(HIGH), debounced_(HIGH), prevDebounced_(HIGH), lastDebounceTime_(0),
      pressStartMs_(0), longPressActive_(false), lastRepeatMs_(0), pressedFlag_(false), releasedFlag_(false)
{
}

void Button::init(uint8_t pin)
{
    pin_ = pin;
    pinMode(pin_, INPUT_PULLUP);
    lastRaw_ = digitalRead(pin_);
    debounced_ = lastRaw_;
    prevDebounced_ = debounced_;
    lastDebounceTime_ = 0;
    pressedFlag_ = false;
    releasedFlag_ = false;
}

void Button::update(uint32_t now)
{
    if (pin_ == 255)
        return; // not initialized

    uint8_t raw = (uint8_t)digitalRead(pin_);
    if (raw != lastRaw_)
    {
        lastDebounceTime_ = now;
        lastRaw_ = raw;
    }

    if ((now - lastDebounceTime_) > config::DEBOUNCE_MS)
    {
        if (raw != debounced_)
        {
            prevDebounced_ = debounced_;
            debounced_ = raw;
            if (debounced_ == LOW && prevDebounced_ == HIGH)
            {
                // falling edge
                pressedFlag_ = true;
                pressStartMs_ = now;
                lastRepeatMs_ = now; // start counting from now so first repeat waits
                longPressActive_ = false;
            }
            else if (debounced_ == HIGH && prevDebounced_ == LOW)
            {
                // rising edge
                releasedFlag_ = true;
                longPressActive_ = false;
            }
        }
    }

    // Handle transition into long-press
    if (!longPressActive_ && debounced_ == LOW && (now - pressStartMs_) >= config::LONG_PRESS_MS)
    {
        longPressActive_ = true;
        lastRepeatMs_ = now; // first repeat is after REPEAT_INTERVAL_MS
    }
}

bool Button::wasPressed()
{
    if (pressedFlag_)
    {
        pressedFlag_ = false;
        return true;
    }
    return false;
}

bool Button::wasReleased()
{
    if (releasedFlag_)
    {
        releasedFlag_ = false;
        return true;
    }
    return false;
}

bool Button::shouldRepeat()
{
    if (!longPressActive_ || debounced_ != LOW)
        return false;
    uint32_t now = (uint32_t)millis();
    if ((now - lastRepeatMs_) >= config::REPEAT_INTERVAL_MS)
    {
        lastRepeatMs_ = now;
        return true;
    }
    return false;
}

bool Button::isPressed() const
{
    return debounced_ == LOW;
}