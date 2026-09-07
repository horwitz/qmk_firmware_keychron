#ifndef CCP_H_INCLUDED
#define CCP_H_INCLUDED

#include "ctrlkeycodes.h"
#include "layout.h"
#include "colorconst.h"

#define HIGH_NIBBLE_DELTA 16 // step size for a high-nibble adjustment (one hex digit's worth)

// GCC statement-expression form: evaluates each argument exactly once, safe for side-effecting expressions
#define min(a,b) \
    ({ __typeof__ (a) min_lhs_ = (a); \
       __typeof__ (b) min_rhs_ = (b); \
       min_lhs_ < min_rhs_ ? min_lhs_ : min_rhs_; })
#define max(a,b) \
    ({ __typeof__ (a) max_lhs_ = (a); \
       __typeof__ (b) max_rhs_ = (b); \
       max_lhs_ > max_rhs_ ? max_lhs_ : max_rhs_; })
// compute a + b, ensuring that the value is at most MAX_COMPONENT
// (assumes b <= MAX_COMPONENT)
#define addBounded(a,b) \
    ({ __typeof__ (a) add_lhs_ = (a); \
       __typeof__ (b) add_rhs_ = (b); \
       add_lhs_ > MAX_COMPONENT - add_rhs_ ? MAX_COMPONENT : add_lhs_ + add_rhs_; })
// compute a - b, ensuring that the value is at least 0
#define subtractBounded(a,b) \
    ({ __typeof__ (a) sub_lhs_ = (a); \
       __typeof__ (b) sub_rhs_ = (b); \
       sub_lhs_ < sub_rhs_ ? 0 : sub_lhs_ - sub_rhs_; })

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

#endif
