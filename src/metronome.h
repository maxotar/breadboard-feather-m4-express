#pragma once
#include <stdint.h>

void metronomeInit();
void metronomeSetBpm(uint8_t bpm);
void metronomeEnable(bool enable);
void metronomeScheduleImmediateBeat();
void metronomeLoop(uint32_t now);

// Query whether a pulse is active (useful for UI decisions)
bool metronomeIsPulseActive();