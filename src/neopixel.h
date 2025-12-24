#pragma once

#include <stdint.h>

// Initialize NeoPixel hardware and apply configured brightness
void neoInit();

// Turn the single onboard pixel blue (on==true) or clear (false)
void neoSetBlue(bool on);