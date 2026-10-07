#pragma once

#include <stdint.h>
#include "colorconst.h"

#define COLOR_PALETTE_SIZE 48
#define GRAY_PALETTE_SIZE  13

// Maps a color palette index [0, COLOR_PALETTE_SIZE) to a hue value in [0, 255].
uint8_t color_hue_for_index(int i);

// Maps a gray palette index [0, GRAY_PALETTE_SIZE) to an intensity value in [0, 255].
uint8_t gray_intensity_for_index(int i);

// scans a rows x cols keymap (flat, row-major) and maps:
//   keycodes in [color_base, color_base + color_count) -> out_color[kc - color_base] = LED index
//   keycodes in [gray_base,  gray_base  + gray_count)  -> out_gray[kc  - gray_base]  = LED index
// hole_map cells equal to 0 are hardware holes and are not counted as LED indices
void scan_palette_keycodes(
    const uint16_t *keymap, const uint16_t *hole_map, int rows, int cols,
    uint16_t color_base, int color_count, uint8_t out_color[],
    uint16_t gray_base,  int gray_count,  uint8_t out_gray[]);
