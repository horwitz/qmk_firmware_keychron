#ifndef GCP_COLOR_H_INCLUDED
#define GCP_COLOR_H_INCLUDED

#include <stdint.h>
#include "colorconst.h"

#define COLOR_PALETTE_SIZE 48
#define GRAY_PALETTE_SIZE  12

// Maps a color palette index [0, COLOR_PALETTE_SIZE) to a hue value in [0, 255].
uint8_t color_hue_for_index(int i);

// Maps a gray palette index [0, GRAY_PALETTE_SIZE) to an intensity value in [0, 255].
uint8_t gray_intensity_for_index(int i);

#endif
