#pragma once
#include <stdint.h>

// Initialize storage; pass the defaultBpm to use if none stored
void storageInit(uint8_t defaultBpm);

// Get currently-known bpm (value from flash or the last scheduled value)
uint8_t storageGetBpm();

// Schedule a delayed save of the provided bpm (debounced / coalesced)
void storageScheduleSave(uint8_t bpm);

// Must be called periodically from loop(now) to perform delayed writes
void storageLoop(uint32_t now);