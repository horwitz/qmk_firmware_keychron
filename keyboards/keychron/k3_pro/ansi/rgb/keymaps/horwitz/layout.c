#include QMK_KEYBOARD_H
#include "layout.h"

// ansi_84_hole_map[row][col] = 1 if that position is an actual key, KC_NO (== 0) if it's a hardware hole
// (84 keys, 12 holes across a 6x16 matrix).
//
// Expanding LAYOUT_ansi_84 with 84 identical non-KC_NO arguments (1 is the simplest choice
// since KC_NO == 0) yields an array where the 12 hardware matrix holes appear as KC_NO.
// Those KC_NO values come from the macro body itself -- not from our arguments -- so
// LAYOUT_ansi_84 (in default_keyboard.h) is the single source of truth for hole positions.
// Row argument counts match LAYOUT_ansi_84's parameter list (84 total, holes excluded).
const uint16_t ansi_84_hole_map[MATRIX_ROWS][MATRIX_COLS] = LAYOUT_ansi_84(
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // row 0: 16 keys, 0 holes
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // row 1: 15 keys, 1 hole  (col 14)
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // row 2: 15 keys, 1 hole  (col 14)
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,      // row 3: 14 keys, 2 holes (cols 12, 14)
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,      // row 4: 14 keys, 2 holes (cols 1, 12)
    1,1,1,1,1,1,1,1,1,1               // row 5: 10 keys, 6 holes (cols 3, 4, 5, 7, 8, 9)
);
