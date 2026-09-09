#pragma once

#include <stdint.h>

// Scans a rows×cols keymap (flat, row-major) against a hole_map and populates out[]
// with the LED indices of non-transparent, non-hole positions, in scan order.
// hole_map cells equal to 0 are hardware holes (not counted toward LED indices).
// Returns the number of entries written to out[].
uint8_t find_used_led_indices(
    uint8_t *out,
    const uint16_t *keymap, const uint16_t *hole_map,
    int rows, int cols,
    uint16_t transparent_kc);
