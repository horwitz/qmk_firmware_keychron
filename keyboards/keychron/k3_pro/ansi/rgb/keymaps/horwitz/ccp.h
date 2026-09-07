#ifndef CCP_H_INCLUDED
#define CCP_H_INCLUDED

#include "ctrlkeycodes.h"

// GCC statement-expression form: evaluates each argument exactly once, safe for side-effecting expressions
#define min(a,b) \
    ({ __typeof__ (a) __min_a = (a); \
       __typeof__ (b) __min_b = (b); \
       __min_a < __min_b ? __min_a : __min_b; })
#define max(a,b) \
    ({ __typeof__ (a) __max_a = (a); \
       __typeof__ (b) __max_b = (b); \
       __max_a > __max_b ? __max_a : __max_b; })
// compute a + b, ensuring that the value is at most 255
// (assumes b <= 255)
#define addBounded(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
    _a > 255 - _b ? 255 : _a + _b; })
// compute a - b, ensuring that the value is at least 0
#define subtractBounded(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
    _a < _b ? 0 : _a - _b; })

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

bool is_ccp_change_keycode(uint16_t keycode);

HSV rgb_to_hsv(RGB rgb);

extern int index_in_byte;

extern RGB ccpRgb;

ccp_key_t get_ccp_key(uint16_t keycode);

layer_state_t layer_state_set_user_ccp(layer_state_t state);

bool process_record_user_ccp(uint16_t keycode, keyrecord_t *record);

void rgb_matrix_indicators_advanced_user_ccp(void);

#endif
