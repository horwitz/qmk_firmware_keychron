/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Features:
 * (1) Debugging
 * (2) Highlighting used fn-layer keys
 * (3) Granular color picking
 * (4) Suspend RGB
 * (5) Complete color picking
 *
 * (1) SHORT NAME†: [DEBUG]
 *     DETAILS: When enabled, outputs the result of `uprintf` statements--these can be seen in the QMK Toolbox console.
 *     TO ACTIVATE: Compile with `DEBUG=1` (e.g. `qmk compile -kb keychron/k3_pro/ansi/rgb -km horwitz DEBUG=1`);
 *                  rules.mk enables CONSOLE_ENABLE automatically. Then use (e.g.) `uprintf` to print debug output.
 *
 * (2) SHORT NAME: [FN-HI]
 *     DETAILS: see fnhi.c
 *     TO ACTIVATE: The feature is always on.
 *
 * (3) SHORT NAME: [GCP]
 *     DETAILS: see gcp.c
 *     TO ACTIVATE: The feature is always on; grayscale mode if off by default--its availability is toggled by double
 *                  tapping X while holding Fn.
 *
 * (4) SHORT NAME: [SUS-RGB]
 *     DETAILS: Turns off lighting when the laptop sleeps (and for similar(?) behavior).
 *     TO ACTIVATE: The feature is always on.
 *
 * (5) SHORT NAME: [CCP]
 *     DETAILS: see ccp.c
 *     TO ACTIVATE: The feature is always on.
 *
 * † notation just for documentation (when a comment with the SHORT NAME is found, the code below--continuing until the
 *   next blank line--is park of the feature with that SHORT NAME (additionally: some single lines have a comment with
 *   the short name at the end of the line, meaning that that one line should be taken into account, rather than
 *   continuing on to the next blank line)... also it is (or at least should be) the case that _all_ of the code for the
 *   feature is commented in that fashion)
 */

#ifndef DEBUG
#define DEBUG 0 // [DEBUG]
#endif

#include QMK_KEYBOARD_H
#include "layers.h"
#include "gcp.h"
#include "ccp.h"
#include "fnhi.h"
#include "keymaps.h"
//#include "ctrlkeycodes.h"
// [DEBUG]
#if DEBUG
    #include "print.h"
#endif

// [SUS-RGB]
void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);
    // suspend_power_down_keymap(); // should this be called?
}
void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
    // suspend_wakeup_init_keymap(); // should this be called?
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_SNAP,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD,MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_macfn,

[WIN_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_winfn,

[GCP] = LAYOUT_gcp, // [GCP]

[CCP] = LAYOUT_ccp // [CCP]
};

void keyboard_post_init_user(void) {
    // [DEBUG]
#if DEBUG
        // Customise these values to desired behaviour
        debug_enable = true;
//        debug_matrix = true;
//        debug_keyboard = true;
//        debug_mouse = true;
#endif

    keyboard_post_init_user_gcp(); // [GCP]

    keyboard_post_init_user_fnhi(); // [FN-HI]
}

// [CCP]
layer_state_t layer_state_set_user(layer_state_t state) {
    return layer_state_set_user_ccp(state);
}

// note potential short circuit
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return (bool)(process_record_user_gcp(keycode, record) && // [GCP]
        process_record_user_ccp(keycode, record)); // [CCP]
}

#if DEBUG
static uint8_t last_layer = -1;
#endif

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = biton32(layer_state);

#if DEBUG
    if (layer != last_layer) {
        uprintf("layer: %d\n", layer);
        last_layer = layer;
    }
#endif

    switch (layer) {
        case MAC_BASE:
        case WIN_BASE:
            break;

        // [FN-HI]
        case MAC_FN:
        case WIN_FN: {
            rgb_matrix_indicators_advanced_user_fnhi(layer);
            break;
        }

        // [GCP]
        case GCP: {
            rgb_matrix_indicators_advanced_user_gcp();
            break;
        }

        // [CCP]
        case CCP: {
            rgb_matrix_indicators_advanced_user_ccp();
            break;
        }
    }
    // the return value here is effectively meaningless: rgb_matrix_indicators_advanced_kb (the weak default,
    // which just calls this function) is itself called from rgb_matrix_indicators_advanced, which is void and
    // ignores the return value. no Keychron override of rgb_matrix_indicators_advanced_kb exists.
    //
    // the red CAPS LOCK indicator visible on the keyboard is also not controlled here. it is a dedicated
    // hardware LED driven by LED_CAPS_LOCK_PIN via writePin() in k3_pro.c's matrix_scan_kb, entirely separate
    // from the RGB matrix pipeline. the RGB matrix CAPS key (index 46) is actually dimmed to black when
    // CAPS LOCK is active, via DIM_CAPS_LOCK in config.h → os_state_indicate() → SET_LED_OFF(CAPS_LOCK_INDEX),
    // called from rgb_matrix_indicators_kb (the bluetooth indicator path), which runs before this function.
    return true;
}
