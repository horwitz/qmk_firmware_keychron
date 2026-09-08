#pragma once

// LED indices for the K3 Pro ANSI 84-key layout (see keyboards/keychron/k3_pro/ansi/rgb/rgb.c)
// indices are assigned by scanning the 6x16 matrix row by row, skipping the 12 hardware holes
// (see ansi_84_hole_map in layout.c)
// row 0 = top row (Esc–RGB_MOD); row 5 = bottom row
#define LED_INDEX_Q    32
#define LED_INDEX_W    33
#define LED_INDEX_E    34
#define LED_INDEX_R    35
#define LED_INDEX_T    36
#define LED_INDEX_Y    37
#define LED_INDEX_I    39
#define LED_INDEX_O    40
#define LED_INDEX_P    41
#define LED_INDEX_A    47
#define LED_INDEX_S    48
#define LED_INDEX_D    49
#define LED_INDEX_F    50
#define LED_INDEX_G    51
#define LED_INDEX_H    52
#define LED_INDEX_K    54
#define LED_INDEX_L    55
#define LED_INDEX_SCLN 56  // ;
#define LED_INDEX_ENT  58
#define LED_INDEX_Z    61
#define LED_INDEX_X    62
#define LED_INDEX_C    63
#define LED_INDEX_V    64
#define LED_INDEX_B    65
#define LED_INDEX_N    66
#define LED_INDEX_COMM 68  // ,
#define LED_INDEX_DOT  69  // .
#define LED_INDEX_SLSH 70  // /
#define LED_INDEX_END  73

// ansi_84_hole_map[row][col] == KC_NO iff that matrix position is a hardware hole (no physical key).
// Defined in layout.c; see there for details.
extern const uint16_t ansi_84_hole_map[MATRIX_ROWS][MATRIX_COLS];
