#ifndef GCP_H_INCLUDED
#define GCP_H_INCLUDED

#include "ctrlkeycodes.h"

// Tap Dance declarations
enum {
    TD_GRAY
};

enum COLOR_SCHEME { RGB_SCHEME, GRAY_SCHEME, UNKNOWN_SCHEME, };

// TODO? (1) derive via sizeof color_picker_color_hues and/or color_picker_color_palette_keycodes OR (2) validate that
//       color_picker_color_palette_keycodes is initialized with an rvalue of COLOR_PALETTE_SIZE elements (see comment
//       above color_picker_color_palette_keycodes's initialization)
#define COLOR_PALETTE_SIZE 48
#define GRAY_PALETTE_SIZE 12

// TODO? do this as a value instead of a define (maybe  const uint16_t PROGMEM ccp_layer_keymap[MATRIX_ROWS][MATRIX_COLS]  ?)
// keycode 73 (End) is lit RED, though pressing any XXXXXXX aborts color choosing
#define LAYOUT_gcp LAYOUT_ansi_84( \
    GRAY00,   GRAY01,   GRAY02,   GRAY03,   GRAY04,   GRAY05,   GRAY06,   GRAY07,   GRAY08,   GRAY09,   GRAY10,   GRAY11,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
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
