#include "gcp_color.h"
#include <math.h>

uint8_t color_hue_for_index(int i) {
    // NB: requires COLOR_PALETTE_SIZE < 512 or the value for the largest i will be 256, wrapping to 0
    return (uint8_t)round(i * HUE_STEPS / (double)COLOR_PALETTE_SIZE);
}

uint8_t gray_intensity_for_index(int i) {
    return (uint8_t)round(i * MAX_COMPONENT / (double)(GRAY_PALETTE_SIZE - 1));
}

void scan_palette_keycodes(
    const uint16_t *keymap, const uint16_t *hole_map, int rows, int cols,
    uint16_t color_base, int color_count, uint8_t out_color[],
    uint16_t gray_base,  int gray_count,  uint8_t out_gray[]) {
    uint8_t led_index = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int i = row * cols + col;
            if (hole_map[i] != 0) {
                uint16_t kc = keymap[i];
                if (kc >= color_base && kc < (uint16_t)(color_base + color_count)) {
                    out_color[kc - color_base] = led_index;
                } else if (kc >= gray_base && kc < (uint16_t)(gray_base + gray_count)) {
                    out_gray[kc - gray_base] = led_index;
                }
                ++led_index;
            }
        }
    }
}
