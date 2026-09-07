#include "gcp_color.h"
#include <math.h>

uint8_t color_hue_for_index(int i) {
    // NB: requires COLOR_PALETTE_SIZE < 512 or the value for the largest i will be 256, wrapping to 0
    return (uint8_t)round(i * HUE_STEPS / (double)COLOR_PALETTE_SIZE);
}

uint8_t gray_intensity_for_index(int i) {
    return (uint8_t)round(i * MAX_COMPONENT / (double)(GRAY_PALETTE_SIZE - 1));
}
