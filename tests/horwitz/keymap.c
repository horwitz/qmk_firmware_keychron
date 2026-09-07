#include "quantum.h"

// This stub satisfies the QMK test framework's requirement for a keymap definition
// (keymap_introspection.c includes KEYMAP_C = "keymap.c" unconditionally for all full tests).
//
// Without it, the build would pick up the firmware's keymap.c from the horwitz keymap
// directory, which is on VPATH so the compiler can find ccp_color.h and colorconst.h.
// That firmware keymap uses QMK types (layer_state_t, keyrecord_t, etc.) that are not
// available in the test environment, causing build failures.
//
// This stub is content-identical to tests/test_common/keymap.c (the shared fallback used
// by all other QMK test suites). It lives here so it shadows the firmware keymap.c in the
// VPATH search order: the test directory is searched before the horwitz firmware directory.

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = {
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},
    },
};

// clang-format on
