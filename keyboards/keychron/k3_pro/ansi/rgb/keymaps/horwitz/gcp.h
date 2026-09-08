#ifndef GCP_H_INCLUDED
#define GCP_H_INCLUDED

#include "ctrlkeycodes.h"
#include "gcp_color.h"

// Tap Dance declarations
enum {
    TD_GRAY
};

enum COLOR_SCHEME { RGB_SCHEME, GRAY_SCHEME, UNKNOWN_SCHEME, };

// TODO? (1) derive via sizeof color_picker_color_hues and/or color_picker_color_palette_keycodes OR (2) validate that
//       color_picker_color_palette_keycodes is initialized with an rvalue of COLOR_PALETTE_SIZE elements (see comment
//       above color_picker_color_palette_keycodes's initialization)

// TODO? do this as a value instead of a define (maybe  const uint16_t PROGMEM ccp_layer_keymap[MATRIX_ROWS][MATRIX_COLS]  ?)
// keycode 73 (End) is lit RED, though pressing any XXXXXXX aborts color choosing
//
// LED index mapping as derived from this layout (for reference; populated at runtime in gcp.c):
//
// color palette (COLOR00–COLOR47):
//   COLOR00 -> LED 16  (`   )  (1) red
//   COLOR01 -> LED 31  (TAB )  (5) scarlet
//   COLOR02 -> LED 46  (CAPS)  (4) vermilion
//   COLOR03 -> LED 60  (LSFT)  (5) persimmon
//   COLOR04 -> LED 17  (1   )  (3) orange
//   COLOR05 -> LED 32  (Q   )  (5) orange peel
//   COLOR06 -> LED 47  (A   )  (4) amber
//   COLOR07 -> LED 61  (Z   )  (5) golden yellow
//   COLOR08 -> LED 18  (2   )  (2) yellow
//   COLOR09 -> LED 33  (W   )  (5) lemon
//   COLOR10 -> LED 48  (S   )  (4) lime
//   COLOR11 -> LED 62  (X   )  (5) spring bud
//   COLOR12 -> LED 19  (3   )  (3) chartreuse
//   COLOR13 -> LED 34  (E   )  (5) bright green
//   COLOR14 -> LED 49  (D   )  (4) harlequin
//   COLOR15 -> LED 63  (C   )  (5) neon green
//   COLOR16 -> LED 20  (4   )  (1) green
//   COLOR17 -> LED 35  (R   )  (5) jade
//   COLOR18 -> LED 50  (F   )  (4) erin
//   COLOR19 -> LED 64  (V   )  (5) emerald
//   COLOR20 -> LED 21  (5   )  (3) spring green
//   COLOR21 -> LED 36  (T   )  (5) mint
//   COLOR22 -> LED 51  (G   )  (4) aquamarine
//   COLOR23 -> LED 65  (B   )  (5) turquoise
//   COLOR24 -> LED 22  (6   )  (2) cyan
//   COLOR25 -> LED 37  (Y   )  (5) sky blue
//   COLOR26 -> LED 52  (H   )  (4) capri
//   COLOR27 -> LED 66  (N   )  (5) cornflower
//   COLOR28 -> LED 23  (7   )  (3) azure
//   COLOR29 -> LED 38  (U   )  (5) cobalt
//   COLOR30 -> LED 53  (J   )  (4) cerulean
//   COLOR31 -> LED 67  (M   )  (5) sapphire
//   COLOR32 -> LED 24  (8   )  (1) blue
//   COLOR33 -> LED 39  (I   )  (5) iris
//   COLOR34 -> LED 54  (K   )  (4) indigo
//   COLOR35 -> LED 68  (,   )  (5) veronica
//   COLOR36 -> LED 25  (9   )  (3) violet
//   COLOR37 -> LED 40  (O   )  (5) amethyst
//   COLOR38 -> LED 55  (L   )  (4) purple
//   COLOR39 -> LED 69  (.   )  (5) phlox
//   COLOR40 -> LED 26  (0   )  (2) magenta
//   COLOR41 -> LED 41  (P   )  (5) fuchsia
//   COLOR42 -> LED 56  (;   )  (4) cerise
//   COLOR43 -> LED 70  (/   )  (5) deep pink
//   COLOR44 -> LED 27  (-   )  (3) rose
//   COLOR45 -> LED 42  ([   )  (5) raspberry
//   COLOR46 -> LED 57  ('   )  (4) crimson
//   COLOR47 -> LED 71  (RSFT)  (5) amaranth
//
// gray palette (GRAY00–GRAY11 -> F1–F12):
//   GRAY00 -> LED  1  (F1 )
//   GRAY01 -> LED  2  (F2 )
//   GRAY02 -> LED  3  (F3 )
//   GRAY03 -> LED  4  (F4 )
//   GRAY04 -> LED  5  (F5 )
//   GRAY05 -> LED  6  (F6 )
//   GRAY06 -> LED  7  (F7 )
//   GRAY07 -> LED  8  (F8 )
//   GRAY08 -> LED  9  (F9 )
//   GRAY09 -> LED 10  (F10)
//   GRAY10 -> LED 11  (F11)
//   GRAY11 -> LED 12  (F12)
#define LAYOUT_gcp LAYOUT_ansi_84( \
    XXXXXXX,  GRAY00,   GRAY01,   GRAY02,   GRAY03,   GRAY04,   GRAY05,   GRAY06,   GRAY07,   GRAY08,   GRAY09,   GRAY10,   GRAY11,   XXXXXXX,  XXXXXXX,  XXXXXXX, \
    COLOR00,  COLOR04,  COLOR08,  COLOR12,  COLOR16,  COLOR20,  COLOR24,  COLOR28,  COLOR32,  COLOR36,  COLOR40,  COLOR44,  XXXXXXX,  XXXXXXX,            XXXXXXX, \
    COLOR01,  COLOR05,  COLOR09,  COLOR13,  COLOR17,  COLOR21,  COLOR25,  COLOR29,  COLOR33,  COLOR37,  COLOR41,  COLOR45,  XXXXXXX,  XXXXXXX,            XXXXXXX, \
    COLOR02,  COLOR06,  COLOR10,  COLOR14,  COLOR18,  COLOR22,  COLOR26,  COLOR30,  COLOR34,  COLOR38,  COLOR42,  COLOR46,            XXXXXXX,            XXXXXXX, \
    COLOR03,            COLOR07,  COLOR11,  COLOR15,  COLOR19,  COLOR23,  COLOR27,  COLOR31,  COLOR35,  COLOR39,  COLOR43,            COLOR47,  XXXXXXX,  XXXXXXX, \
    XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX \
)

void keyboard_post_init_user_gcp(void);

bool process_record_user_gcp(uint16_t keycode, const keyrecord_t *record);

void rgb_matrix_indicators_advanced_user_gcp(void);

#endif
