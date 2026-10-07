#include "fnhi_color.h"

uint8_t find_used_led_indices(
    uint8_t *out,
    const uint16_t *keymap, const uint16_t *hole_map,
    int rows, int cols,
    uint16_t transparent_kc) {
    uint8_t count = 0;
    uint8_t led_index = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int i = row * cols + col;
            if (hole_map[i] != 0) {
                if (keymap[i] != transparent_kc) {
                    out[count++] = led_index;
                }
                ++led_index;
            }
        }
    }
    return count;
}
