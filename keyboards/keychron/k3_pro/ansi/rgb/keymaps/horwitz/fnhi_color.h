#pragma once

#include <stdint.h>

// Scans a rows×cols keymap (flat, row-major) against a hole_map and populates out[]
// with the LED indices of non-transparent, non-hole positions, in scan order.
// hole_map cells equal to 0 are hardware holes (not counted toward LED indices).
// Returns the number of entries written to out[].
//
// Uses the same matrix-scan pattern as scan_palette_keycodes (gcp_color.h); a shared
// abstraction (DRY) seems like the wrong direction--the payloads differ enough that
// a callback would probably add too much complexity.
uint8_t find_used_led_indices(
    uint8_t *out,
    const uint16_t *keymap, const uint16_t *hole_map,
    int rows, int cols,
    uint16_t transparent_kc);
