#pragma once

// LED indices for the K3 Pro ANSI 84-key layout (see keyboards/keychron/k3_pro/ansi/rgb/rgb.c)
#define LED_INDEX_END 73

// ansi_84_hole_map[row][col] == KC_NO iff that matrix position is a hardware hole (no physical key).
// Defined in layout.c; see there for details.
extern const uint16_t ansi_84_hole_map[MATRIX_ROWS][MATRIX_COLS];