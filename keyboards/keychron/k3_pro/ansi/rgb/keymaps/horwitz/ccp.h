#pragma once

#include "ctrlkeycodes.h"
#include "layout.h"
#include "ccp_color.h"

#define HIGH_NIBBLE_DELTA 16 // step size for a high-nibble adjustment (one hex digit's worth)

enum RGB_COLOR { RED, GREEN, BLUE };
enum NIBBLE_LEVEL { HIGH, LOW };
enum DELTA_DIR { INC, DEC };

typedef struct {
    enum RGB_COLOR color;
    enum NIBBLE_LEVEL nibbleLevel;
    enum DELTA_DIR deltaDir;
} ccp_key_t;

// TODO? do this as a value instead of a define (maybe  const uint16_t PROGMEM ccp_layer_keymap[MATRIX_ROWS][MATRIX_COLS]  ?)
#define LAYOUT_ccp LAYOUT_ansi_84( \
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX, \
    XXXXXXX,  RHI,      RLI,      GHI,      GLI,      BHI,      BLI,      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX, \
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            CCPSET,             XXXXXXX, \
    XXXXXXX,            RHD,      RLD,      GHD,      GLD,      BHD,      BLD,      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  CCPABORT, \
    XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX \
)

extern int8_t index_in_byte;

extern RGB ccpRgb;

layer_state_t layer_state_set_user_ccp(layer_state_t state);

bool process_record_user_ccp(uint16_t keycode, const keyrecord_t *record);

void rgb_matrix_indicators_advanced_user_ccp(void);