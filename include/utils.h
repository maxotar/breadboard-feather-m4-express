#pragma once
#include <stdint.h>

// Scale BPM (0..maxBpm) to a bits-wide integer (0..(2^bits-1)), rounded
static inline int scaleBpmToBits(uint8_t bpmVal, uint8_t maxBpm, uint8_t bits)
{
    if (bits == 0 || maxBpm == 0)
        return 0;
    int maxVal = (1 << bits) - 1;
    return ((int)bpmVal * maxVal + maxBpm / 2) / maxBpm;
}